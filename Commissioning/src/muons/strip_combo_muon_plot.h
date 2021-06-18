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
                if (ps_.find({tdc1, tdc2}) != ps_.end())
                {
                    canvas()->Divide(1, 2);
                    canvas()->cd(1);
                    ps_[{tdc1, tdc2}].Draw("HIST");
                    canvas()->cd(2);
                    ps_[{tdc2, tdc1}].Draw("HIST");
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
                        int tdc1 = std::min(hit1.tdc(), hit2.tdc());
                        int tdc2 = std::max(hit1.tdc(), hit2.tdc());
                        if (ps_.find({tdc1, tdc2}) == ps_.end()) // TDC pair not yet made
                        {
                            ps_.emplace(std::piecewise_construct,
                                       std::make_tuple(tdc1, tdc2),
                                       std::make_tuple(Form("%s_%d_%d", name_, tdc1, tdc2), "", 32, 0, 32));
                            ps_[{tdc1, tdc2}].GetXaxis()->SetTitle("Strips");
                        
                            ps_.emplace(std::piecewise_construct,
                                       std::make_tuple(tdc2, tdc1),
                                       std::make_tuple(Form("%s_%d_%d", name_, tdc2, tdc1), "", 32, 0, 32));
                            ps_[{tdc2, tdc1}].GetXaxis()->SetTitle("Strips");
                        }
                        ps_[{tdc1, tdc2}].Fill(hit1.strip());
                        ps_[{tdc2, tdc1}].Fill(hit2.strip());
                    }
                }
            }
        }
    }

private:
    std::map<std::pair<int, int>, TH1F> ps_;
    double min_timing_;
};