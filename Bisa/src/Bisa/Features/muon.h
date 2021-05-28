#pragma once

#include "HitCollection.h"

namespace Bisa
{

class Muon : public HitCollection
{
public:
    Muon()
     : HitCollection()
    {
    }

    virtual ~Muon() {}

    void add(const Hit &hit)
    {
        BA_CORE_ASSERT(hit.trigger_id() == trigger_id_ || size() == 0, 
                       "Adding new hit with different trigger ID to Cluster");

        BA_CORE_ASSERT(hit.coordinate() == coordinate_ || size() == 0, 
                       "Adding new hit with different coordinate to Cluster");

        BA_CORE_ASSERT(hit.chamber() == chamber_ || size() == 0,
                       "Adding new hit with different chamber to Cluster");

        hits_.insert(hit);
        trigger_id_ = hit.trigger_id();
        coordinate_ = hit.coordinate();
        chamber_ = hit.chamber();
    }

    Coordinate coordinate() const { return coordinate_; }
    unsigned int chamber() const { return chamber_; }
    std::set<unsigned int> layers() const
    {
        std::set<unsigned int> ls;
        for (auto &&hit : hits_)
        {
            ls.insert(hit.layer());
        }
        return ls;
    }

protected:
    Coordinate coordinate_;
    unsigned int chamber_ = 10;
};

}