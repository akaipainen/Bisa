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

    ~Cluster() {}

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
    }

    Coordinate coordinate() const { return coordinate_; }
    unsigned int layer() const { return layer_; }
    unsigned int chamber() const { return chamber_; }

    int max_strip_hit() const
    {
        BA_CORE_ASSERT(!hits_.empty(), "Getting hits from non-empty cluster");
        int max_hit = -1;
        for (auto &&hit : hits_)
        {
            if (hit.strip() > max_hit)
            {
                max_hit = hit.strip();
            }
        }
        return max_hit;
    }

    int min_strip_hit() const
    {
        BA_CORE_ASSERT(!hits_.empty(), "Getting hits from non-empty cluster");
        int min_hit = 100;
        for (auto &&hit : hits_)
        {
            if (hit.strip() < min_hit)
            {
                min_hit = hit.strip();
            }
        }
        return min_hit;
    }

    double min_time() const
    {
        BA_CORE_ASSERT(!hits_.empty(), "Getting hits from non-empty cluster");
        double min_time = 32 * 25;
        for (auto &&hit : hits_)
        {
            if (hit.time() < min_time)
            {
                min_time = hit.time();
            }
        }
        return min_time;
    }

protected:
    Coordinate coordinate_;
    unsigned int layer_ = 10;
    unsigned int chamber_ = 10;
};

}