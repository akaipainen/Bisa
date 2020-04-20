#pragma once

#include "Bisa/Core.h"

#include <unordered_map>
#include <string>
#include <sstream>

namespace Bisa {

    struct Hit
    {
        unsigned int triggerId;
        unsigned int bcidFpga;
        unsigned int felixCounter;

        unsigned int tdc;
        unsigned int channel;
        unsigned int width;
        unsigned int bcidTdc;
        unsigned int fineTime;

        unsigned int strip;
        unsigned int uniqueId;

        Hit()
         : uniqueId(UniqueIdCounter++)
        {
        }

        Hit(unsigned int triggerId, 
            unsigned int bcidFpga,
            unsigned int felixCounter,
            unsigned int tdc,
            unsigned int channel,
            unsigned int width,
            unsigned int bcidTdc,
            unsigned int fineTime)
         : triggerId(triggerId)
         , bcidFpga(bcidFpga)
         , felixCounter(felixCounter)
         , tdc(tdc)
         , channel(channel)
         , width(width)
         , bcidTdc(bcidTdc)
         , fineTime(fineTime)
         , uniqueId(UniqueIdCounter++)
        {
        }

        std::string toString()
        {
            std::stringstream ret;
            ret << "Unique ID: " << uniqueId;
            ret << ", Trigger ID: " << triggerId;
            ret << ", TDC: " << tdc;
            ret << ", Channel: " << channel;
            ret << ", BCID (TDC): " << bcidTdc;
            ret << ", Fine Time: " << fineTime;
            return ret.str();
        }

        bool operator==(const Hit& other)
        {
            return uniqueId == other.uniqueId;
        }
        bool operator!=(const Hit& other)
        {
            return !operator==(other);
        }

    private:
        static unsigned int UniqueIdCounter;
    };

    class BISA_API HitCollection
    {
    public:
        HitCollection() = default;
        virtual ~HitCollection() {}

        void add(Ref<Hit> hit);
        void remove(Ref<Hit> hit);

        unsigned int triggerId() { return triggerId_; }
        unsigned int size() { return hits_.size(); }
        
        void add(const HitCollection& hits);
        void remove(const HitCollection& hits);

        HitCollection operator&(const HitCollection& other); // intersection
        HitCollection operator|(const HitCollection& other); // add/union
        HitCollection operator-(const HitCollection& other); // subtract

        std::string toString();

        std::unordered_map<unsigned int, Ref<Hit>>::iterator begin() { return hits_.begin(); }
        std::unordered_map<unsigned int, Ref<Hit>>::iterator end() { return hits_.end(); }

        std::unordered_map<unsigned int, Ref<Hit>>::const_iterator begin() const { return hits_.begin(); }
        std::unordered_map<unsigned int, Ref<Hit>>::const_iterator end() const { return hits_.end(); }

    private:
        std::unordered_map<unsigned int, Ref<Hit>> hits_;
        unsigned int triggerId_;
    };

}
