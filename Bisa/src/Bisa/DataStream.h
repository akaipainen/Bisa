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
        using NewHitCallbackFn = std::function<void(Hits&)>;

        DataStream(const StreamProps& props);
        virtual ~DataStream();

        void SetNewDataCallback(const NewHitCallbackFn& callback) { m_NewHitCallback = callback; }

        bool FillNextEvent();

    private:
        virtual void Init(const StreamProps& props);
        virtual void Shutdown();

        void FillBufferWithNextPacket();
        
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
        bool m_PairMode;
        std::fstream m_DataFile;
        std::deque<Hits> m_HitsBuffer;

        NewHitCallbackFn m_NewHitCallback;

        unsigned int m_NumberOfPackets;
        bool m_FileFinished = false;
    };

}

