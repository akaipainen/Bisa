#pragma once

#include "Bisa/Core.h"

#include <vector>
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

        std::string ToString()
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

    class BISA_API Hits
    {
    public:
        Hits() = default;
        ~Hits();
        Hits(const Hits& other);
        Hits& operator=(const Hits& other);

        void PushHit(Hit* hit);
        void PopHit(Hit* hit);

        void Merge(const Hits& hits);

        unsigned int TriggerId() { return m_TriggerId; }

        std::string ToString();

        unsigned int size() { return size_first(); }
        unsigned int size_first() { return m_FirstHits.size(); }
        unsigned int size_add() { return m_AdditionalHits.size(); }

        std::vector<Hit*>::iterator begin() { return begin_first(); }
        std::vector<Hit*>::iterator end() { return end_first(); }

        std::vector<Hit*>::iterator begin_first() { return m_FirstHits.begin(); }
        std::vector<Hit*>::iterator end_first() { return m_FirstHits.end(); }

        std::vector<Hit*>::iterator begin_add() { return m_AdditionalHits.begin(); }
        std::vector<Hit*>::iterator end_add() { return m_AdditionalHits.end(); }

        std::vector<Hit*>::const_iterator begin() const { return begin_first(); }
        std::vector<Hit*>::const_iterator end() const { return end_first(); }

        std::vector<Hit*>::const_iterator begin_first() const { return m_FirstHits.begin(); }
        std::vector<Hit*>::const_iterator end_first() const { return m_FirstHits.end(); }

        std::vector<Hit*>::const_iterator begin_add() const { return m_AdditionalHits.begin(); }
        std::vector<Hit*>::const_iterator end_add() const { return m_AdditionalHits.end(); }

    private:
        std::vector<Hit*> m_FirstHits;
        std::vector<Hit*> m_AdditionalHits;

        unsigned int m_TriggerId;
    };

}
