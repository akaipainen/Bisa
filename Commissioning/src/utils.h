#pragma once
#include <Bisa.h>

namespace utils
{
    inline bool hits_same_chamber(const Bisa::Hit &hit1, const Bisa::Hit &hit2, const Bisa::Config &config)
    {
        if (config.chamber(hit1.tdc()) == config.chamber(hit2.tdc()))
        {
            return true;
        }
        return false;
    }
 
    inline bool hits_same_layer(const Bisa::Hit &hit1, const Bisa::Hit &hit2, const Bisa::Config &config)
    {
        if (config.layer(hit1.tdc()) == config.layer(hit2.tdc()))
        {
            return true;
        }
        return false;
    }

    inline bool hits_same_coord(const Bisa::Hit &hit1, const Bisa::Hit &hit2, const Bisa::Config &config)
    {
        
        if (config.coordinate(hit1.tdc()) == config.coordinate(hit2.tdc()))
        {
            return true;
        }
        return false;
    }

    inline bool hits_same_coord_layer(const Bisa::Hit &hit1, const Bisa::Hit &hit2, const Bisa::Config &config)
    {
        if (hits_same_chamber(hit1, hit2, config))
        {
            if (hits_same_coord(hit1, hit2, config))
            {
                if (config.layer(hit1.tdc()) == config.layer(hit2.tdc()))
                {
                    return true;
                }
            }
        }
        return false;
    }

    inline int hits_signed_dist_apart(const Bisa::Hit &hit1, const Bisa::Hit &hit2, const Bisa::Config &config)
    {
        return (config.orientation(hit2.tdc()) * 32 + config.strip(hit2.channel())) - (config.orientation(hit1.tdc()) * 32 + config.strip(hit1.channel()));
    }

    inline bool hits_dist_apart(const Bisa::Hit &hit1, const Bisa::Hit &hit2, const Bisa::Config &config, unsigned int distance)
    {
        if (abs(hits_signed_dist_apart(hit1, hit2, config)) <= distance)
        {
            return true;
        }
        return false;
    }

    inline bool hits_time_apart(const Bisa::Hit &hit1, const Bisa::Hit &hit2, const Bisa::Config &config, unsigned int time)
    {
        if (abs(config.time(hit1.bcid_tdc(), hit1.fine_time()) - config.time(hit2.bcid_tdc(), hit2.fine_time())) <= time)
        {
            return true;
        }
        return false;
    }

    inline double hits_time_apart(const Bisa::Hit &hit1, const Bisa::Hit &hit2, const Bisa::Config &config)
    {
        return abs(config.time(hit1.bcid_tdc(), hit1.fine_time()) - config.time(hit2.bcid_tdc(), hit2.fine_time()));
    }

} // namespace utils
