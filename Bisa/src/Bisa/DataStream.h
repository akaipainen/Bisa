#pragma once

#include "bapch.h"

#include "Bisa/Core.h"
#include "Bisa/Features/Hits.h"

namespace Bisa {

    struct BISA_API StreamProps
    {
        std::string filename;
        bool pairmode;

        StreamProps(const std::string& filename = "run.dat",
                    bool pairmode = true)
         : filename(filename)
         , pairmode(pairmode)
        {
        }
    };

    class BISA_API DataStream
    {
    public:
        using NewEventCallbackFn = std::function<void(Hits&)>;

        DataStream(const StreamProps& props);
        virtual ~DataStream();

        void SetNewDataCallback(const NewEventCallbackFn& callback) { m_NewEventCallback = callback; }

        bool FillNextEvent();

    private:
        virtual void Init(const StreamProps& props);
        virtual void Shutdown();

        void FillBufferWithNextPacket();

        class IdCounter
        {
        public:
            IdCounter(int max=255)
             : m_Max(max)
             , m_LastValue(0)
             , m_LastTrue(0)
            {
            }

            int get(int value)
            {
                // In case this is first value
                if (m_LastValue == 0 && m_LastTrue == 0)
                {
                    m_LastValue = value;
                    m_LastTrue = value;
                    return value;
                }
                if (value < m_LastValue) // New value is smaller
                {
                    if (m_LastValue - value > m_Max / 2) // Rolled over (255 -> 0)
                        m_LastTrue += m_Max + value - m_LastValue;
                    else // Else, decreased normally (Ex. 133 -> 132)
                        m_LastTrue += value - m_LastValue;
                    
                }
                else // New value is larger
                {
                    if (value - m_LastValue > m_Max / 2) // Rolled back (0 -> 255)
                        m_LastTrue += value - m_LastValue - m_Max;
                    else // Else, increased normally (Ex. 123 -> 124)
                        m_LastTrue += value - m_LastValue;
                }
                m_LastValue = value;
                return m_LastTrue;
            }
        private:
            unsigned int m_Max;
            unsigned int m_LastValue;
            unsigned int m_LastTrue;
        };
        
        struct Packet
        {
            std::string data;
            unsigned int numWords;
            
            Packet(const std::string& hexData)
             : data(hexData)
            {
                numWords = (data.size()/2 - 20) / 4;
            }

            unsigned long Word(unsigned int i)
            {
                return Bits(26 + i * 8, 8);
            }

            unsigned long FpgaHeader()
            {
                return Word(numWords);
            }

            static unsigned long Slice(unsigned long bits, unsigned int shift, unsigned int length)
            {
                return bits >> shift & (1 << length) - 1;
            }

            unsigned long Bits(unsigned int start, unsigned int length)
            {
                BA_CORE_ASSERT(length <= 8, "Cannot return more than 8 bytes (32 bits)!");
                BA_CORE_ASSERT(start < data.size(), "Start must be before end of data string!");
                std::stringstream ss(data.substr(start, length));
                unsigned long ret;
                ss >> std::hex >> ret;
                return ret;
            }
        };

        Hits DecodePacket(Packet packet);

    private:
        std::fstream m_DataFile;
        std::deque<Hits> m_HitsBuffer;
        IdCounter m_IdCounter;
        bool m_PairMode;

        NewEventCallbackFn m_NewEventCallback;

        unsigned int m_NumberOfPackets;
        bool m_FileFinished = false;
    };

}

