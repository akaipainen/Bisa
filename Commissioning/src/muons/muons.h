#pragma once

#include <Bisa.h>

#include "../Selector.h"
#include "../basic/strip_distribution_plot.h"
#include "counter_plot.h"

class Muons : public Bisa::Experiment
{
public:
    Muons(const char *name, TTree *tree, const char *output_path);

    ~Muons();

    void add_hits(const Bisa::HitCollection &hits);

    static Bisa::FeatureCollection<Bisa::Muon> create_muons(const Bisa::HitCollection &hits)
    {
        Bisa::FeatureCollection<Bisa::Cluster> clusters = Selector::clusterize(hits);
        Bisa::FeatureCollection<Bisa::Muon> muons;

        for (auto cluster1_it = clusters.begin(); cluster1_it != clusters.end(); cluster1_it++)
        {
            auto cluster1 = *cluster1_it;
            if (cluster1.size() <= Bisa::config.exparam()["muons"]["cluster_size"])
            {
                for (auto cluster2_it = std::next(cluster1_it); cluster2_it != clusters.end(); cluster2_it++)
                {
                    auto cluster2 = *cluster2_it;
                    if (cluster2.size() <= Bisa::config.exparam()["muons"]["cluster_size"])
                    {
                        if (cluster1.chamber() == cluster2.chamber() &&
                            cluster1.coordinate() == cluster2.coordinate() &&
                            cluster1.layer() != cluster2.layer() &&
                            std::abs(cluster1.min_time() - cluster2.min_time()) <= Bisa::config.exparam()["muons"]["timing_ns"] &&
                            cluster1.distance(cluster2) < Bisa::config.exparam()["muons"]["cluster_distance"])
                        {
                            if (!Bisa::config.exparam()["muons"]["require_3_layers"]) // require 2 layers
                            {
                                Bisa::Muon muon;
                                for (auto &&hit : cluster1.hits() | cluster2.hits())
                                {
                                    muon.add(hit);
                                }
                                muons.add(muon);
                            }
                            else // require 3 layers
                            {
                                for (auto cluster3_it = std::next(cluster2_it); cluster3_it != clusters.end(); cluster3_it++)
                                {
                                    auto cluster3 = *cluster3_it;
                                    if (cluster3.chamber() == cluster1.chamber() &&
                                        cluster3.coordinate() == cluster1.coordinate() &&
                                        cluster3.size() <= Bisa::config.exparam()["muons"]["cluster_size"] &&
                                        cluster3.layer() != cluster1.layer() && cluster3.layer() != cluster2.layer() &&
                                        std::abs(cluster3.min_time() - cluster2.min_time()) <= Bisa::config.exparam()["muons"]["timing_ns"] &&
                                        std::abs(cluster3.min_time() - cluster1.min_time()) <= Bisa::config.exparam()["muons"]["timing_ns"] &&
                                        cluster3.distance(cluster1) < Bisa::config.exparam()["muons"]["cluster_distance"] &&
                                        cluster3.distance(cluster2) < Bisa::config.exparam()["muons"]["cluster_distance"])
                                    {
                                        Bisa::Muon muon;
                                        for (auto &&hit : cluster1.hits() | cluster2.hits() | cluster3.hits())
                                        {
                                            muon.add(hit);
                                        }
                                        muons.add(muon);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return muons;
    }

protected:
    
    StripDistributionPlot muon_distribution_;
    CounterPlot muon_count_;
};
