#include "Bisa/Features/HitCollection.h"

namespace Bisa {

    void HitCollection::add(const Hit& new_hit)
    {
        BA_CORE_ASSERT(new_hit.trigger_id() == trigger_id_ || size() == 0, 
                       "Adding new hit with different Trigger ID to HitCollection");
        
        // Only add the new Hit if not already in Collection
        auto found = hit_ids_.find(new_hit.unique_id());
        if (found == hit_ids_.end()) 
        {
            hits_.emplace_back(new_hit);
            hit_ids_.emplace(new_hit.unique_id());
            trigger_id_ = new_hit.trigger_id();
        }
    }

    void HitCollection::clear()
    {
        hit_ids_.clear();
        hits_.clear();
    }

    HitCollection HitCollection::operator&(const HitCollection& other) const
    {
        HitCollection hc;
        const HitCollection& smaller = other.hits_.size() < hits_.size() ? other : *this;
        const HitCollection& bigger = other.hits_.size() < hits_.size() ? *this : other;
        for (auto itHit = smaller.begin(); itHit != smaller.end(); itHit++)
        {
            if (bigger.hit_ids_.find(itHit->unique_id()) != bigger.hit_ids_.end())
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
            hc.add(*itHit);
        }
        return hc;
    }

    HitCollection HitCollection::operator-(const HitCollection& other) const
    {
        HitCollection hc;
        for (auto itHit = begin(); itHit != end(); itHit++)
        {
            if (other.hit_ids_.find(itHit->unique_id()) == other.hit_ids_.end())
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