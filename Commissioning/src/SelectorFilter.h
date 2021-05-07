#include <Bisa.h>
#include "utils.h"

namespace SelectorFilter
{
    inline bool muon_candidate(const Bisa::Hit& hit1, const Bisa::Hit& hit2, const Bisa::Config& config)
    {
        if (utils::hits_same_chamber(hit1, hit2, config))
        {
            if (utils::hits_same_coord(hit1, hit2, config))
            {
                if (utils::hits_dist_apart(hit1, hit2, config, 1))
                {
                    if (utils::hits_time_apart(hit1, hit2, config, 5))
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    inline bool adjacent_hits(Bisa::Hit hit1, Bisa::Hit hit2, const Bisa::Config& config)
    {
        if (utils::hits_same_chamber(hit1, hit2, config))
        {
            if (utils::hits_same_coord_layer(hit1, hit2, config))
            {
                if (utils::hits_dist_apart(hit1, hit2, config, 1))
                {
                    return true;
                }
            }
        }
        return false;
    }

    inline bool in_time_hits(const Bisa::Hit& hit1, const Bisa::Hit& hit2, const Bisa::Config& config)
    {
        if (utils::hits_time_apart(hit1, hit2, config, 5))
        {
            return true;
        }
        return false;
    }

    inline bool adjacent_hits_across_layers(const Bisa::Hit& hit1, const Bisa::Hit& hit2, const Bisa::Config& config)
    {
        if (utils::hits_same_chamber(hit1, hit2, config))
        {
            if (utils::hits_same_coord(hit1, hit2, config))
            {
                if (utils::hits_dist_apart(hit1, hit2, config, 1))
                {
                    return true;
                }
            }
        }
        return false;
    }


    class SWTriggerCandidate
    {
    public:
        SWTriggerCandidate(unsigned int time, unsigned int strip_distance)
        : time_(time)
        , strip_distance_(strip_distance)
        { }
    
        bool operator()(const Bisa::Hit& hit1, const Bisa::Hit& hit2, const Bisa::Config& config) const
        {
            if (utils::hits_same_chamber(hit1, hit2, config))
            {
                if (utils::hits_same_coord(hit1, hit2, config))
                {
                    if (utils::hits_time_apart(hit1, hit2, config, time_))
                    {
                        if (utils::hits_dist_apart(hit1, hit2, config, strip_distance_))
                        {
                            return true;
                        }
                    }
                }
            }
            return false;
        }
    private:
        unsigned int time_;
        unsigned int strip_distance_;
    };



}