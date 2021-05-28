#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TString.h>
#include <TStyle.h>

#include "../Selector.h"

class EfficiencyPlot : public Bisa::Plot
{
public:
    EfficiencyPlot(const char *name, const char *title, int voltage, int layer, Bisa::Experiment *experiment)
     : Bisa::Plot(name, title, experiment)
     , p_(Form("%s_%d", name, voltage), title, 15, 4500, 6000)
     , voltage_(voltage)
     , layer_(layer)
    {
        p_.GetXaxis()->SetTitle("Voltage [V]");
        p_.GetYaxis()->SetTitle("Efficiency [%]");

        experiment_->tree()->Branch(Form("%s_%d", name, voltage), &trigger_);
    }

    ~EfficiencyPlot() 
    {
        // BA_INFO(1.0 / event_counter_);
        gStyle->SetOptStat(0);
        p_.Scale(100.0 / tag_counter_); // Get percentage for efficiency
        canvas()->cd(); // switch to canvas
        p_.Draw("HIST"); // draw this plot to canvas
        print(); // save plot to pdf
    }

    void add_hits(const Bisa::HitCollection &hits)
    {
        trigger_ = tag_probe(hits);
        if (trigger_ >= 0) tag_counter_++;
        if (trigger_ == 1) p_.Fill(voltage_);
    }

private:
    // Return 1 if test found, 0 if no test, -1 if no tag
    int tag_probe(const Bisa::HitCollection &hits)
    {
        Bisa::FeatureCollection<Bisa::Cluster> clusters = Selector::clusterize(hits);

        unsigned int tag_layer_1 = layer_ != 0 ? 0 : 1;
        unsigned int tag_layer_2 = layer_ != 2 ? 2 : 1;
        bool tag_found = false;
        for (auto &&cluster1 : clusters)
        {
            if ((cluster1.chamber() == 7 || cluster1.chamber() == 8) &&
                cluster1.layer() == tag_layer_1 &&
                cluster1.size() <= Bisa::config.exparam()["efficiency"]["tag_cluster_size"])
            {
                for (auto &&cluster2 : clusters)
                {
                    if (cluster2.chamber() == cluster1.chamber() &&
                        cluster2.layer() == tag_layer_2 &&
                        cluster2.coordinate() == cluster1.coordinate() &&
                        cluster2.size() <= Bisa::config.exparam()["efficiency"]["tag_cluster_size"])
                    {
                        // Spatial requirement for tag clusters
                        if (cluster1.min_strip_hit() - cluster2.max_strip_hit() <= Bisa::config.exparam()["efficiency"]["tag_cluster_distance"] ||
                            cluster2.min_strip_hit() - cluster1.max_strip_hit() <= Bisa::config.exparam()["efficiency"]["tag_cluster_distance"])
                        {
                            // Timing requirement for tag clusters
                            if (std::abs(cluster1.min_time() - cluster2.min_time()) <= Bisa::config.exparam()["efficiency"]["tag_timing_ns"])
                            {
                                tag_found = true;
                                for (auto &&cluster3 : clusters) // Search for probe hit
                                {
                                    if (cluster3.layer() == layer_ &&
                                        cluster3.chamber() == cluster1.chamber() &&
                                        cluster3.coordinate() == cluster1.coordinate() &&
                                        cluster3.size() <= Bisa::config.exparam()["efficiency"]["probe_cluster_size"])
                                    {
                                        // Spatial+Timing requirement for probe hit
                                        if ((cluster3.min_strip_hit() - cluster2.max_strip_hit() <= Bisa::config.exparam()["efficiency"]["probe_cluster_distance"] ||
                                             cluster2.min_strip_hit() - cluster3.max_strip_hit() <= Bisa::config.exparam()["efficiency"]["probe_cluster_distance"]) &&
                                            (cluster1.min_strip_hit() - cluster3.max_strip_hit() <= Bisa::config.exparam()["efficiency"]["probe_cluster_distance"] ||
                                             cluster3.min_strip_hit() - cluster1.max_strip_hit() <= Bisa::config.exparam()["efficiency"]["probe_cluster_distance"]))
                                        {
                                            // Timing requirement for tag clusters
                                            if (std::abs(cluster3.min_time() - cluster2.min_time()) <= Bisa::config.exparam()["efficiency"]["probe_timing_ns"] &&
                                                std::abs(cluster3.min_time() - cluster1.min_time()) <= Bisa::config.exparam()["efficiency"]["probe_timing_ns"])
                                            {
                                                return 1;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }   
            }
        }
        if (tag_found) return 0;
        return -1;
    }

    TH1F p_;
    int voltage_;
    int layer_;
    int trigger_;

    int tag_counter_ = 0;
};