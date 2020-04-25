#include "Bisa/Features/HitCollection.h"

namespace Bisa {

    void HitCollection::add(Ref<Hit> new_hit)
    {
        BA_CORE_ASSERT(new_hit->trigger_id() == trigger_id_ || size() == 0, 
                       "Adding new hit with different Trigger ID to HitCollection");
        
        // Only add the new Hit if not already in Collection
        auto found = hit_ids_.find(new_hit->unique_id());
        if (found == hit_ids_.end()) 
        {
            hits_.emplace_back(new_hit);
            hit_ids_.emplace(new_hit->unique_id());
            trigger_id_ = new_hit->trigger_id();
        }
    }

    void HitCollection::remove(Iterator hit_it)
    {
        std::list<Ref<Hit>>::iterator pit = hit_it;
        hits_.erase(pit);
    }

    void HitCollection::remove(ConstIterator hit_it)
    {
        std::list<Ref<Hit>>::const_iterator pit = hit_it;
        hits_.erase(pit);
    }

    void HitCollection::remove(Ref<Hit> hit)
    {
        // If this feature exists in this collection, find it and remove it
        auto search = hit_ids_.find(hit->unique_id());
        if (search != hit_ids_.end())
        {
            auto found = std::find(begin(), end(), *hit);
            remove(found);
        }
    }

    void HitCollection::add(const HitCollection& other)
    {
        for (auto hit_it = other.begin(); hit_it != other.end(); hit_it++)
        {
            add(hit_it.get());
        }
    }

    void HitCollection::remove(const HitCollection& other)
    {
        for (auto hit_it = other.begin(); hit_it != other.end(); hit_it++)
        {
            remove(hit_it);
        }
    }

    HitCollection HitCollection::operator&(const HitCollection& other) const
    {
        HitCollection hc;
        const HitCollection& smaller = other.hits_.size() < hits_.size() ? other : *this;
        const HitCollection& bigger = other.hits_.size() < hits_.size() ? *this : other;
        for (auto hit_it = smaller.begin(); hit_it != smaller.end(); hit_it++)
        {
            if (bigger.hit_ids_.find(hit_it->unique_id()) != bigger.hit_ids_.end())
            {
                hc.add(hit_it.get());
            }
        }
        return hc;
    }

    HitCollection HitCollection::operator|(const HitCollection& other) const
    {
        HitCollection hc;
        hc.add(*this);
        hc.add(other);
        return hc;
    }

    HitCollection HitCollection::operator-(const HitCollection& other) const
    {
        HitCollection hc(*this);
        hc.remove(other);
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