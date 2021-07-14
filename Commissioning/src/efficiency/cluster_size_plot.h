#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TString.h>
#include <TStyle.h>

#include "../Selector.h"

class ClusterSizePlot : public Bisa::Plot
{
public:
    ClusterSizePlot(const char *name, const char *title, int voltage, Bisa::Experiment *experiment)
     : Bisa::Plot(name, title, experiment)
     , p_(Form("%s_%d", name, voltage), title, 32, 0, 32)
     , voltage_(voltage)
    {
        p_.GetXaxis()->SetTitle("Cluster size [strips]");

        // experiment_->tree()->Branch(Form("%s_%d", name, voltage), &trigger_);
    }

    ~ClusterSizePlot() 
    {
        // BA_INFO(1.0 / event_counter_);
        gStyle->SetOptStat(0);
        canvas()->cd(); // switch to canvas
        p_.Draw("HIST"); // draw this plot to canvas
        print(); // save plot to pdf
    }

    void add_hits(const Bisa::HitCollection &hits)
    {
        Bisa::FeatureCollection<Bisa::Cluster> clusters = Selector::clusterize(hits);
        for (auto &cluster : clusters)
        {
            p_.Fill(cluster.size());
        }
    }

private:
    TH1F p_;
    int voltage_;
};