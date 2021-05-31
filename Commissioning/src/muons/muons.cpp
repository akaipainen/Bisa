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

    bool eta_muon = false;
    bool phi_muon = false;
    for (auto &&muon : muons)
    {
        if (muon.coordinate() == Bisa::ETA) eta_muon = true;
        if (muon.coordinate() == Bisa::PHI) phi_muon = true;
    }
    
    if (Bisa::config.exparam()["muons"]["eta_and_phi"] && eta_muon && phi_muon)
    {
        muon_distribution_.add_hits(muons.hits());
        muon_count_.increment();
    }
    else if (!Bisa::config.exparam()["muons"]["eta_and_phi"] && (eta_muon || phi_muon))
    {
        muon_distribution_.add_hits(muons.hits());
        muon_count_.increment();
    }
}