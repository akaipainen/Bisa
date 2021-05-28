#pragma once
#include "bapch.h"

#include "Bisa/Config.h"
#include "Bisa/Features/HitCollection.h"
#include "Packet.h"

namespace Bisa {

    class BISA_API DataStream
    {
    public:
        using NewEventCallbackFn = std::function<void(HitCollection&)>;

        DataStream(const char *datafile);
        virtual ~DataStream();

        void set_new_data_callback(const NewEventCallbackFn& callback) { new_event_callback_ = callback; }

        bool fill_next_event();

    private:

        void fill_buffer_with_next_packet();

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

        HitCollection decode_packet(Packet packet);

    private:
        std::fstream data_file_;
        std::deque<HitCollection> hits_buffer_;
        IdCounter id_counter_;
        bool pairmode_;

        NewEventCallbackFn new_event_callback_;

        unsigned int number_of_packets_;
        bool file_finished_ = false;

        unsigned int running_id_;
    };

}

