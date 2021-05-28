#pragma once

#include "HitCollection.h"

namespace Bisa
{

class Cluster : public HitCollection
{
public:
    Cluster()
     : HitCollection()
    {
    }

    virtual ~Cluster() {}

    void add(const Hit &hit) override
    {
        BA_CORE_ASSERT(hit.trigger_id() == trigger_id_ || size() == 0, 
                       "Adding new hit with different trigger ID to Cluster");

        BA_CORE_ASSERT(hit.coordinate() == coordinate_ || size() == 0, 
                       "Adding new hit with different coordinate to Cluster");

        BA_CORE_ASSERT(hit.layer() == layer_ || size() == 0,
                       "Adding new hit with different layer to Cluster");

        BA_CORE_ASSERT(hit.chamber() == chamber_ || size() == 0,
                       "Adding new hit with different chamber to Cluster");
        
        // Only add the new Hit if not already in Collection

        hits_.insert(hit);
        trigger_id_ = hit.trigger_id();
        coordinate_ = hit.coordinate();
        layer_ = hit.layer();
        chamber_ = hit.chamber();

        max_strip_ = std::max(max_strip_, hit.rpc_strip());
        min_strip_ = std::min(min_strip_, hit.rpc_strip());
        min_time_ = std::min(min_time_, hit.time());
    }

    Coordinate coordinate() const { return coordinate_; }
    unsigned int layer() const { return layer_; }
    unsigned int chamber() const { return chamber_; }

    unsigned int distance(const Cluster &other) const
    {
        int d = std::min(min_strip_hit() - other.max_strip_hit(), other.min_strip_hit() - max_strip_hit());
        if (d < 0) return 0;
        return d;
    }

    int max_strip_hit() const
    {
        return max_strip_;
    }

    int min_strip_hit() const
    {
        return min_strip_;
    }

    double min_time() const
    {
        return min_time_;
    }

    HitCollection hits() const
    {
        return *this;
    }

protected:
    Coordinate coordinate_;
    unsigned int layer_ = 10;
    unsigned int chamber_ = 10;
    unsigned int max_strip_ = 0;
    unsigned int min_strip_ = 100;
    double min_time_ = 32 * 25;
};

}