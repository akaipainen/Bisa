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
        pairMode_ = props.pairmode;

        BA_CORE_INFO("Creating data stream from file: {0}", props.filename);

        dataFile_.open(props.filename);
        BA_CORE_ASSERT(dataFile_.is_open(), "Could not open file!")

        idCounter_ = IdCounter(255);
    }

    void DataStream::Shutdown()
    {
    }

    bool DataStream::FillNextEvent()
    {
        while (!fileFinished_ && hitsBuffer_.size() < 10)
        {
            // BA_CORE_INFO("Hits buffer size: {}", hitsBuffer_.size());
            FillBufferWithNextPacket();
        }
        if (hitsBuffer_.size() > 0)
        {
            newEventCallback_(hitsBuffer_.front());
            hitsBuffer_.pop_front();
            return true;
        }
        return false;        
    }

    void DataStream::FillBufferWithNextPacket()
    {
        // Find the start of the next packet.
        // If no next packet exists, set file finished to true
        char c;
        while (dataFile_.get(c))
        {
            if (c == '=') break;
        }
        if (c != '=')
        {
            fileFinished_ = true;
            return;
        }
        
        // Read packet into data
        int numBytes;
        dataFile_ >> numBytes;
        std::string data;
        std::string byte;
        for (int i = 0; i < numBytes; i++)
        {
            dataFile_ >> byte;
            data += byte;
        }
        Packet packet(data);
        
        // Some error checking
        bool ok = true;
        if (data[2*numBytes - 2] != 'a' || data[2*numBytes - 1] != 'a')
        {
            BA_CORE_WARN("Skipping packet ({0}...): Missing trailing 'aa'", data.substr(0, 15));
            ok = false;
        }
        // If number of bytes is < 24, this is an error. Log it
        if (numBytes < 24) {
            BA_CORE_WARN("Skipping packet ({0}...): {1} bytes", data.substr(0, 15), numBytes);
            ok = false;
        }
        // If number of bytes is > 10000, this is an error. Log it
        if (numBytes > 1000) {
            BA_CORE_WARN("Skipping packet ({0}...): {1} bytes", data.substr(0, 15), numBytes);
            ok = false;
        }

        // If no errors, continue
        // Otherwise, move onto next packet
        if (!ok) return FillBufferWithNextPacket();

        HitCollection hits = DecodePacket(packet);
        auto it = hitsBuffer_.begin();
        for (; it != hitsBuffer_.end(); it++)
        {
            if (hits.triggerId() == it->triggerId())
            {
                // BA_CORE_TRACE("Merging hits with Trigger ID = {} in buffer", hits.triggerId());
                it->add(hits);
                break;
            }
        }
        if (it == hitsBuffer_.end())
        {
            // BA_CORE_TRACE("Adding packet with Trigger ID = {} to buffer", hits.triggerId());
            hitsBuffer_.emplace_back(hits);
        }
    }

    HitCollection DataStream::DecodePacket(Packet packet)
    {
        HitCollection hits;
        for (auto wordId = 0; wordId < packet.numWords; wordId++)
        {
            Hit newHit;
            newHit.triggerId = idCounter_.get(Packet::Slice(packet.FpgaHeader(), 16, 8));
            newHit.bcidFpga = Packet::Slice(packet.FpgaHeader(), 0, 16);
            newHit.felixCounter = packet.Bits(26+(packet.numWords+1)*8, 2);

            newHit.tdc = Packet::Slice(packet.Word(wordId), 24, 4);
            newHit.channel = Packet::Slice(packet.Word(wordId), 19, 5);
            newHit.width = pairMode_ ? Packet::Slice(packet.Word(wordId), 12, 7) : 0;
            newHit.bcidTdc = pairMode_ ? Packet::Slice(packet.Word(wordId), 7, 5) : Packet::Slice(packet.Word(wordId), 7, 12);
            newHit.fineTime = Packet::Slice(packet.Word(wordId), 0, 7);

            // newHit.strip

            // BA_CORE_TRACE("Added new hit to hits ({})", newHit.toString());
            hits.add(CreateRef<Hit>(newHit));
        }
        // BA_CORE_TRACE("Finished decoding packet ({})", hits.toString());
        return hits;
    }

}