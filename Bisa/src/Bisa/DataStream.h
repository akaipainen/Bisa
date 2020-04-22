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

        class Packet {
        public:
            Packet() // default ctor
             : raw_data_(new char[1]), size_(1)
            {
            }

            Packet(const char *packet_data)  // ctor
             : size_(strlen(packet_data))
            {
                raw_data_ = new char[size_ + 1];
                strcpy(raw_data_, packet_data);
                raw_data_[size_] = '\0';
            }

            Packet(const Packet &other) // copy ctor
             : size_(other.size_)
            {
                raw_data_ = new char[size_ + 1];
                strcpy(raw_data_, other.raw_data_);
            }

            Packet(Packet &&other) // move ctor
             : raw_data_(nullptr), size_(0)
            {
                size_ = other.size_;
                raw_data_ = other.raw_data_;

                other.raw_data_ = nullptr;
                other.size_ = 0;
            }

            ~Packet() // dtor
            {
                delete[] raw_data_;
            }

            Packet & operator=(const Packet &rhs) // copy assign
            {
                if (this != &rhs)
                {
                    Packet temp(rhs);
                    std::swap(raw_data_, temp.raw_data_);
                    std::swap(size_, temp.size_);
                }
                return *this;
            }

            Packet & operator=(Packet && rhs) // move assign
            {
                if (this != &rhs)
                {
                    delete[] raw_data_;
                    raw_data_ = rhs.raw_data_;
                    size_ = rhs.size_;

                    rhs.raw_data_ = nullptr;
                    rhs.size_ = 0;
                }
                return *this;
            }

            unsigned long Word(unsigned int i)
            {
                return raw_bits(26 + i * 8, 8);
            }

            unsigned long numWords()
            {
                return (size_/2 - 20) / 4;
            }

            unsigned long FpgaHeader()
            {
                return Word(numWords());
            }

            static unsigned long Slice(unsigned long bits, unsigned int shift, unsigned int length)
            {
                return bits >> shift & (1 << length) - 1;
            }

            unsigned long raw_bits(size_t start, size_t length) const
            {
                BA_CORE_ASSERT(start < size_, "Start location of read raw bits is past end");
                
                char * start_ptr = raw_data_ + start;
                char temp[length + 1];
                strncpy(temp, start_ptr, length);
                temp[length] = '\0';
                // printf("%s = %lu\n", temp, strtoul(temp, nullptr, 16));
                return strtoul(temp, nullptr, 16);
            }

        private:
            char * raw_data_;
            unsigned int size_;
            bool fragment_;
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

