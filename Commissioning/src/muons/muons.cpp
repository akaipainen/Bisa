#include "muons.h"

Muons::Muons(const char *name, TTree *tree, const char *output_path)
 : Bisa::Experiment(name, tree, output_path)
 , muon_distribution_("muon_distribution", "Muon Distribution Plot", this)
 , muon_count_("muon_count", "Number of Muons detected", this)
{
}

Muons::~Muons()
{
}

void Muons::add_hits(const Bisa::HitCollection &hits)
{
    Bisa::FeatureCollection<Bisa::Muon> muons = create_muons(hits);

    Bisa::FeatureCollection<Bisa::Muon> eta_phi_muons;
    for (auto &&muon1 : muons)
    {
        if (muon1.coordinate() == Bisa::ETA)
        {
            for (auto &&muon2 : muons)
            {
                if (muon2.coordinate() == Bisa::PHI)
                {
                    if (std::abs(muon1.min_time() - muon2.min_time()) <= Bisa::config.exparam()["muons"]["timing_ns"])
                    {
                        eta_phi_muons.add(muon1);
                        eta_phi_muons.add(muon2);
                        break;
                    }
                }
            }
        }
    }
    
    if (Bisa::config.exparam()["muons"]["eta_and_phi"] && eta_phi_muons.size() > 0)
    {
        muon_distribution_.add_hits(eta_phi_muons.hits());
        muon_count_.increment();
    }
    else if (!Bisa::config.exparam()["muons"]["eta_and_phi"] && muons.size() > 0)
    {
        muon_distribution_.add_hits(muons.hits());
        muon_count_.increment();
    }
}