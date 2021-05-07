#include "SWTrigger.h"

#include "utils.h"

bool SWTrigger::simple(const Bisa::HitCollection& hits)
{
    if (hits.size() == 0) return false;
    
    auto min_fpga_time = hits.begin()->bcid_fpga();
    for (auto &&hit : hits)
    {
        if (hit.bcid_fpga() < min_fpga_time)
        {
            min_fpga_time = hit.bcid_fpga();
        }
    }

    for (auto &&hit1 : hits)
    {
        for (auto &&hit2 : hits)
        {
            if (utils::hits_same_coord(hit1, hit2, config_))
            {
                // Hits 1 and 2 are in FPGA time window
                if (hit1.bcid_fpga() <= min_fpga_time + 2)
                {
                    if (hit2.bcid_fpga() <= min_fpga_time + 2)
                    {
                        // Different layers
                        if (!utils::hits_same_layer(hit1, hit2, config_))
                        {
                            // Max 2 strips apart
                            if (utils::hits_dist_apart(hit1, hit2, config_, 2))
                            {
                                // Max 25 ns apart
                                if (utils::hits_time_apart(hit1, hit2, config_, 25))
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool SWTrigger::pad(const Bisa::HitCollection& hits)
{
    for (auto &&hit1 : hits)
    {
        for (auto &&hit2 : hits)
        {
            if (hit1 != hit2) // Different hits
            {
                if (utils::hits_same_coord(hit1, hit2, config_)) // Same coord
                {
                    if (!utils::hits_same_layer(hit1, hit2, config_)) // Different layer
                    {
                        if (utils::hits_time_apart(hit1, hit2, config_, 13)) // Time apart
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}