#pragma once

#include "bapch.h"

#include "Bisa/Core.h"
#include "Bisa/Features/HitCollection.h"

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
        using NewEventCallbackFn = std::function<void(HitCollection&)>;

        DataStream(const StreamProps& props);
        virtual ~DataStream();

        void SetNewDataCallback(const NewEventCallbackFn& callback) { newEventCallback_ = callback; }

        bool FillNextEvent();

    private:
        virtual void Init(const StreamProps& props);
        virtual void Shutdown();

        void FillBufferWithNextPacket();

        class IdCounter
        {
        public:
            IdCounter(int max=255)
             : max_(max)
             , lastValue_(0)
             , lastTrue_(0)
            {
            }

            int get(int value)
            {
                // In case this is first value
                if (lastValue_ == 0 && lastTrue_ == 0)
                {
                    lastValue_ = value;
                    lastTrue_ = value;
                    return value;
                }
                if (value < lastValue_) // New value is smaller
                {
                    if (lastValue_ - value > max_ / 2) // Rolled over (255 -> 0)
                        lastTrue_ += max_ + value - lastValue_;
                    else // Else, decreased normally (Ex. 133 -> 132)
                        lastTrue_ += value - lastValue_;
                    
                }
                else // New value is larger
                {
                    if (value - lastValue_ > max_ / 2) // Rolled back (0 -> 255)
                        lastTrue_ += value - lastValue_ - max_;
                    else // Else, increased normally (Ex. 123 -> 124)
                        lastTrue_ += value - lastValue_;
                }
                lastValue_ = value;
                return lastTrue_;
            }
        private:
            unsigned int max_;
            unsigned int lastValue_;
            unsigned int lastTrue_;
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

        HitCollection DecodePacket(Packet packet);

    private:
        std::fstream dataFile_;
        std::deque<HitCollection> hitsBuffer_;
        IdCounter idCounter_;
        bool pairMode_;

        NewEventCallbackFn newEventCallback_;

        unsigned int numberOfPackets_;
        bool fileFinished_ = false;
    };

}

