#include "Bisa/Features/HitCollection.h"

namespace Bisa {

    void HitCollection::add(const Hit& new_hit)
    {
        BA_CORE_ASSERT(new_hit.trigger_id() == trigger_id_ || size() == 0, 
                       "Adding new hit with different Trigger ID to HitCollection");
        
        // Only add the new Hit if not already in Collection

        hits_.insert(new_hit);
        trigger_id_ = new_hit.trigger_id();
    }

    void HitCollection::clear()
    {
        hits_.clear();
    }

    // std::set<unsigned int> HitCollection::layers() const
    // {
    //     std::set<unsigned int> layers_;
    //     for (auto &&hit : *this)
    //     {
    //         layers_.insert(config.layer(hit.tdc()));
    //     }
    //     return layers_;
    // }

    HitCollection HitCollection::operator&(const HitCollection& other) const
    {
        HitCollection hc;
        const HitCollection& smaller = other.hits_.size() < hits_.size() ? other : *this;
        const HitCollection& bigger = other.hits_.size() < hits_.size() ? *this : other;
        for (auto itHit = smaller.begin(); itHit != smaller.end(); itHit++)
        {
            if (bigger.hits_.find(*itHit) != bigger.hits_.end())
            {
                hc.add(*itHit);
            }
        }
        return hc;
    }

    HitCollection HitCollection::operator|(const HitCollection& other) const
    {
        HitCollection hc(*this);
        for (auto itHit = other.begin(); itHit != other.end(); itHit++)
        {
            if (hits_.find(*itHit) == hits_.end())
            {
                hc.add(*itHit);
            }
        }
        return hc;
    }

    HitCollection HitCollection::operator-(const HitCollection& other) const
    {
        HitCollection hc;
        for (auto itHit = begin(); itHit != end(); itHit++)
        {
            if (other.hits_.find(*itHit) == other.hits_.end())
            {
                hc.add(*itHit);
            }
        }
        return hc;
    }

    std::string HitCollection::to_string() const
    {
        std::stringstream ret;
        ret << "Trigger ID: " << trigger_id_;
        ret << ", Number of hits: " << size();
        return ret.str();
    }

}