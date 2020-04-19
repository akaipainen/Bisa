#include "bapch.h"
#include "Bisa/DataStream.h"

namespace Bisa {
    
    DataStream::DataStream(const StreamProps& props)
    {
        Init(props);
    }

    DataStream::~DataStream()
    {
        Shutdown();
    }

    void DataStream::Init(const StreamProps& props)
    {   
        m_PairMode = props.pairmode;

        BA_CORE_INFO("Creating data stream from file: {0}", props.filename);

        m_DataFile.open(props.filename);
        BA_CORE_ASSERT(m_DataFile.is_open(), "Could not open file!")
    }

    void DataStream::Shutdown()
    {
    }

    bool DataStream::FillNextEvent()
    {
        while (!m_FileFinished && m_HitsBuffer.size() < 10)
        {
            BA_CORE_INFO("Hits buffer size: {}", m_HitsBuffer.size());
            FillBufferWithNextPacket();
        }
        if (m_HitsBuffer.size() > 0)
        {
            m_NewHitCallback(m_HitsBuffer.front());
            m_HitsBuffer.pop_front();
            return true;
        }
        return false;        
    }

    void DataStream::FillBufferWithNextPacket()
    {
        // Find the start of the next packet.
        // If no next packet exists, set file finished to true
        char c;
        while (m_DataFile.get(c))
        {
            if (c == '=') break;
        }
        if (c != '=')
        {
            m_FileFinished = true;
            return;
        }
        
        // Read packet into data
        int numBytes;
        m_DataFile >> numBytes;
        std::string data;
        std::string byte;
        for (int i = 0; i < numBytes; i++)
        {
            m_DataFile >> byte;
            data += byte;
        }
        Packet packet(data);
        
        // Some error checking
        bool ok = true;
        if (data[2*numBytes - 2] != 'a' || data[2*numBytes - 1] != 'a')
        {
            BA_CORE_WARN("Skipping packet ({0:10s}...): Missing trailing 'aa'", data);
            ok = false;
        }
        // If number of bytes is < 24, this is an error. Log it
        if (numBytes < 24) {
            BA_CORE_WARN("Skipping packet ({0:10s}...): {1} bytes", data, numBytes);
            ok = false;
        }
        // If number of bytes is > 10000, this is an error. Log it
        if (numBytes > 1000) {
            BA_CORE_WARN("Skipping packet ({0:10s}...): {1} bytes", data, numBytes);
            ok = false;
        }

        // If no errors, continue
        // Otherwise, move onto next packet
        if (!ok) return FillBufferWithNextPacket();

        Hits hits = DecodePacket(packet);
        auto it = m_HitsBuffer.begin();
        for (; it != m_HitsBuffer.end(); it++)
        {
            if (hits.TriggerId() == it->TriggerId())
            {
                BA_CORE_TRACE("Merging hits with Trigger ID = {} in buffer", hits.TriggerId());
                it->Merge(hits);
                break;
            }
        }
        if (it == m_HitsBuffer.end())
        {
            BA_CORE_TRACE("Adding packet with Trigger ID = {} to buffer", hits.TriggerId());
            m_HitsBuffer.emplace_back(hits);
        }
    }

    Hits DataStream::DecodePacket(Packet packet)
    {
        Hits hits;
        for (auto wordId = 0; wordId < packet.numWords; wordId++)
        {
            Hit newHit;
            newHit.triggerId = Packet::Slice(packet.FpgaHeader(), 16, 8);
            newHit.bcidFpga = Packet::Slice(packet.FpgaHeader(), 0, 16);
            newHit.felixCounter = packet.Bits(26+(packet.numWords+1)*8, 2);

            newHit.tdc = Packet::Slice(packet.Word(wordId), 24, 4);
            newHit.channel = Packet::Slice(packet.Word(wordId), 19, 5);
            newHit.width = m_PairMode ? Packet::Slice(packet.Word(wordId), 12, 7) : 0;
            newHit.bcidTdc = m_PairMode ? Packet::Slice(packet.Word(wordId), 7, 5) : Packet::Slice(packet.Word(wordId), 7, 12);
            newHit.fineTime = Packet::Slice(packet.Word(wordId), 0, 7);

            // newHit.strip

            BA_CORE_TRACE("Added new hit to hits ({})", newHit.ToString());
            hits.PushHit(new Hit(newHit));
        }
        BA_CORE_TRACE("Finished decoding packet ({})", hits.ToString());
        return hits;
    }

}