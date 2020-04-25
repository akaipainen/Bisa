#include "Packet.h"

namespace Bisa {

    // Default constructor
        Packet::Packet() 
         : raw_data_(new char[1]), size_(1)
        {
        }

        Packet::Packet(const char *packet_data)
         : size_(strlen(packet_data))
        {
            raw_data_ = new char[size_ + 1];
            strcpy(raw_data_, packet_data);
            raw_data_[size_] = '\0';
        }

        Packet::Packet(const Packet &other)
         : size_(other.size_)
        {
            raw_data_ = new char[size_ + 1];
            strcpy(raw_data_, other.raw_data_);
        }

        Packet::Packet(Packet &&other)
         : raw_data_(nullptr), size_(0)
        {
            size_ = other.size_;
            raw_data_ = other.raw_data_;

            other.raw_data_ = nullptr;
            other.size_ = 0;
        }

        Packet::~Packet()
        {
            delete[] raw_data_;
        }

        Packet& Packet::operator=(const Packet &rhs)
        {
            if (this != &rhs)
            {
                Packet temp(rhs);
                std::swap(raw_data_, temp.raw_data_);
                std::swap(size_, temp.size_);
            }
            return *this;
        }

        Packet& Packet::operator=(Packet && rhs)
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

        unsigned long Packet::word(unsigned int i)
        {
            return raw_bits(26 + i * 8, 8);
        }

        unsigned long Packet::num_words()
        {
            return (size_/2 - 20) / 4;
        }

        unsigned long Packet::fpga_header()
        {
            return word(num_words());
        }

        unsigned long Packet::slice(unsigned long bits, unsigned int shift, unsigned int length)
        {
            return bits >> shift & (1 << length) - 1;
        }

        unsigned long Packet::raw_bits(unsigned int start, unsigned int length) const
        {
            BA_CORE_ASSERT(start < size_, "Start location of read raw bits is past end");
            
            char * start_ptr = raw_data_ + start;
            char temp[length + 1];
            strncpy(temp, start_ptr, length);
            temp[length] = '\0';
            // printf("%s = %lu\n", temp, strtoul(temp, nullptr, 16));
            return strtoul(temp, nullptr, 16);
        }

}