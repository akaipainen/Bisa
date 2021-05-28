#pragma once

#include <Bisa.h>

#include "../basic/strip_distribution_plot.h"
#include "../Selector.h"

class MuonDistributionPlot : public StripDistributionPlot
{
public:
    MuonDistributionPlot(const char *name, const char *title, Bisa::Experiment *experiment)
     : StripDistributionPlot(name, title, experiment)
    {
    }

    virtual ~MuonDistributionPlot() {}

    virtual void add_hits(const Bisa::HitCollection &hits) override
    {
        Bisa::FeatureCollection<Bisa::Muon> muons = create_muons(hits);
        
        bool eta_muon = false;
        bool phi_muon = false;
        for (auto &&muon : muons)
        {
            if (muon.coordinate() == Bisa::ETA) eta_muon = true;
            if (muon.coordinate() == Bisa::PHI) phi_muon = true;
        }
        
        if (Bisa::config.exparam()["muons"]["eta_and_phi"] && eta_muon && phi_muon)
        {
            for (auto &&hit : muons.hits())
            {
                ps_[hit.tdc()].Fill(hit.strip());
            }
        }
        else if (eta_muon || phi_muon)
        {

        }
    }
};
