#include "efficiency.h"

#include "../Selector.h"

Efficiency::Efficiency(const char *name, int voltage, TTree *tree, const char *output_path)
 : Bisa::Experiment(name, tree, output_path)
 , voltage_(voltage)
 , efficiency_top_("efficiency_top", "Efficiency Top", voltage, 0, this)
 , efficiency_mid_("efficiency_mid", "Efficiency Mid", voltage, 1, this)
 , efficiency_bot_("efficiency_bot", "Efficiency Bot", voltage, 2, this)
 , efficiency_top_eta_("efficiency_top_eta", "Efficiency Top Eta", voltage, 0, this)
 , efficiency_mid_eta_("efficiency_mid_eta", "Efficiency Mid Eta", voltage, 1, this)
 , efficiency_bot_eta_("efficiency_bot_eta", "Efficiency Bot Eta", voltage, 2, this)
 , efficiency_top_phi_("efficiency_top_phi", "Efficiency Top Phi", voltage, 0, this)
 , efficiency_mid_phi_("efficiency_mid_phi", "Efficiency Mid Phi", voltage, 1, this)
 , efficiency_bot_phi_("efficiency_bot_phi", "Efficiency Bot Phi", voltage, 2, this)
{
}

Efficiency::~Efficiency()
{
}

void Efficiency::add_hits(const Bisa::HitCollection &hits)
{
    auto eta_hits = select_eta(hits);
    auto phi_hits = select_phi(hits);

    efficiency_top_.add_hits(hits);
    efficiency_mid_.add_hits(hits);
    efficiency_bot_.add_hits(hits);

    efficiency_top_eta_.add_hits(eta_hits);
    efficiency_mid_eta_.add_hits(eta_hits);
    efficiency_bot_eta_.add_hits(eta_hits);

    efficiency_top_phi_.add_hits(phi_hits);
    efficiency_mid_phi_.add_hits(phi_hits);
    efficiency_bot_phi_.add_hits(phi_hits);
}

Bisa::HitCollection Efficiency::select_eta(const Bisa::HitCollection& hits) const
{
    Bisa::HitCollection hc;
    for (auto &&hit : hits)
    {
        if (hit.coordinate() == Bisa::ETA)
        {
            hc.add(hit);
        }
    }
    return hc;
}

Bisa::HitCollection Efficiency::select_phi(const Bisa::HitCollection& hits) const
{
    Bisa::HitCollection hc;
    for (auto &&hit : hits)
    {
        if (hit.coordinate() == Bisa::PHI)
        {
            hc.add(hit);
        }
    }
    return hc;
}