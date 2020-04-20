#include "bapch.h"

#include "Bisa/Features/HitCollection.h"

namespace Bisa {

    unsigned int Hit::UniqueIdCounter = 0;

    void HitCollection::add(Ref<Hit> newHit)
    {
        // Gains ownership of hit
        BA_CORE_ASSERT(newHit->triggerId == triggerId_ || size() == 0, 
                       "Adding new hit with different Trigger ID to HitCollection");
        
        auto found = hits_.find(newHit->uniqueId);
        if (found == hits_.end()) // if not found, add it
        {
            hits_.insert({newHit->uniqueId, newHit});
            triggerId_ = newHit->triggerId;
        }
    }

    void HitCollection::remove(Ref<Hit> hit)
    {
        auto found = hits_.find(hit->uniqueId);
        if (found != hits_.end()) // if found, 
        {
            hits_.erase(found);
        }
    }

    void HitCollection::add(const HitCollection& other)
    {
        for (auto it = other.begin(); it != other.end(); it++)
        {
            hits_.insert(*it);
        }
    }

    void HitCollection::remove(const HitCollection& other)
    {
        for (auto it = other.begin(); it != other.end(); it++)
        {
            auto found = hits_.find(it->first);
            if (found != hits_.end())
            {
                hits_.erase(found);
            }
        }
    }

    HitCollection HitCollection::operator&(const HitCollection& other)
    {
        HitCollection hc;
        const HitCollection& smaller = other.hits_.size() < hits_.size() ? other : *this;
        const HitCollection& bigger = other.hits_.size() < hits_.size() ? *this : other;
        for (auto &&hitPair : smaller)
        {
            if (bigger.hits_.find(hitPair.first) != bigger.end())
            {
                hc.add(hitPair.second);
            }
        }
        return hc;
    }

    HitCollection HitCollection::operator|(const HitCollection& other)
    {
        HitCollection hc;
        hc.add(*this);
        hc.add(other);
        return hc;
    }

    HitCollection HitCollection::operator-(const HitCollection& other)
    {
        HitCollection hc(*this);
        hc.remove(other);
        return hc;
    }

    std::string HitCollection::toString()
        {
            std::stringstream ret;
            ret << "Trigger ID: " << triggerId_;
            ret << ", Number of hits: " << size();
            return ret.str();
        }

}