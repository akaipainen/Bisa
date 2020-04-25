#pragma once
#include "bapch.h"

namespace Bisa {

    class Packet {
    public:
        // Default constructor
        Packet();

        // Constructor from raw packet data
        Packet(const char *packet_data);

        // Copy constructor
        Packet(const Packet &other);

        // Move constructor
        Packet(Packet &&other);

        // Destructor
        ~Packet();

        // Copy assignment operator
        Packet & operator=(const Packet &rhs);

        // Move assignment operator
        Packet & operator=(Packet && rhs);

        // Get the i-th word from datawords in packet
        // Returned as unsigned long in bits
        unsigned long word(unsigned int i);

        // Return number of datawords in packet
        unsigned long num_words();

        // Return fpga header bits as unsigned long
        unsigned long fpga_header();

        // Helper static method to slice bits according to a shift and length
        static unsigned long slice(unsigned long bits, unsigned int shift, unsigned int length);

        // Helper method to extract raw bits from raw data packet data
        unsigned long raw_bits(unsigned int start, unsigned int length) const;

    private:
        char * raw_data_;
        unsigned int size_;
        bool fragment_;
    };

}
