#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TString.h>
#include <TStyle.h>

#include "../Selector.h"

class StripComboMuonPlot : public Bisa::Plot
{
public:
    StripComboMuonPlot(const char *name, const char *title, Bisa::Experiment *experiment)
     : Bisa::Plot(name, title, experiment)
    {   
        canvas()->SetTitle(title);
    }

    ~StripComboMuonPlot() 
    {
        gStyle->SetOptStat(0);

        for (int tdc1 = 0; tdc1 < 18; tdc1++)
        {
            for (int tdc2 = tdc1 + 1; tdc2 < 18; tdc2++)
            {
                auto p_ = ps_.find({tdc1, tdc2});
                if (p_ != ps_.end())
                {
                    canvas()->cd();
                    p_->second.LabelsDeflate("X"); // Delete any bins that are not present
                    p_->second.LabelsOption("av"); // sort by increasing value
                    p_->second.Draw("HIST");
                    print(Form("%s_%d_%d", name_, tdc1, tdc2), name_);
                    canvas()->Clear();
                }
            }
        }
    }

    void add_hits(const Bisa::HitCollection &hits)
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
                            cluster1.distance(cluster2) <= Bisa::config.exparam()["muons"]["cluster_distance"])
                        {
                            Bisa::Muon muon;
                            for (auto &&hit : cluster1.hits() | cluster2.hits())
                            {
                                muon.add(hit);
                            }
                            muons.add(muon);
                        }
                    }
                }
            }
        }

        for (auto &&muon : muons)
        {
            for (auto &&hit1 : muon)
            {
                for (auto &&hit2 : muon)
                {
                    if (hit1.tdc() < hit2.tdc())
                    {
                        int tdc1 = hit1.tdc();
                        int tdc2 = hit2.tdc();
                        if (ps_.find({tdc1, tdc2}) == ps_.end()) // TDC pair not yet made
                        {
                            ps_.emplace(std::piecewise_construct,
                                       std::make_tuple(tdc1, tdc2),
                                       std::make_tuple(Form("%s_%d_%d", name_, tdc1, tdc2), "", 1024, 0, 1024));
                            ps_[{tdc1, tdc2}].GetXaxis()->SetTitle("Strips");
                            ps_[{tdc1, tdc2}].SetCanExtend(TH1::kAllAxes);
                        }
                        ps_[{tdc1, tdc2}].Fill(Form("%d-%d", hit1.strip(), hit2.strip()), 1);
                    }
                }
            }
        }
    }

private:
    std::map<std::pair<int, int>, TH1F> ps_;
    double min_timing_;
};