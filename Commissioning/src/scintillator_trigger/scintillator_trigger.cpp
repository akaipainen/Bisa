#include "scintillator_trigger.h"

ScintillatorTrigger::ScintillatorTrigger(const char *name, TTree *tree, const Bisa::Config &config)
 : Bisa::Experiment(name, tree, config)
//  , check_trigger_("check_pad_trigger", "Reconstructed PAD Triggers", this, config)
 , scint_fpga_timing_("scint_fpga_timing_jitter", "Maximum FPGA Timing Jitter between Scintillator Trigger Hits", this, config)
 , chamber_fpga_timing_("chamber_fpga_timing_jitter", "Maximum FPGA Timing Jitter between Chamber Hits", this, config)
 , check_pad_trigger_("check_pad_trigger_w_scint", "Percentage of PAD Triggers with scintillator trigger", this, config)
 , check_pad_trigger_eta_("check_pad_trigger_w_scint_eta", "Percentage of eta PAD triggers with scintillator trigger", this, config)
 , check_pad_trigger_phi_("check_pad_trigger_w_scint_phi", "Percentage of phi PAD triggers with scintillator trigger", this, config)
{
}

ScintillatorTrigger::~ScintillatorTrigger()
{
}

void ScintillatorTrigger::add_hits(const Bisa::HitCollection &hits)
{
    Bisa::HitCollection scint_hits = on_tdcs(hits, {15, 17});
    Bisa::HitCollection chamber_hits = hits - scint_hits;

    scint_fpga_timing_.add_hits(scint_hits);
    chamber_fpga_timing_.add_hits(chamber_hits);

    check_pad_trigger_.add_hits(chamber_hits);

    Bisa::HitCollection eta_hits = on_coord(chamber_hits, Bisa::ETA);
    check_pad_trigger_eta_.add_hits(eta_hits);
    
    Bisa::HitCollection phi_hits = on_coord(chamber_hits, Bisa::PHI);
    check_pad_trigger_phi_.add_hits(phi_hits);
}

Bisa::HitCollection ScintillatorTrigger::on_tdcs(const Bisa::HitCollection &hits, const std::set<unsigned int> &tdcs)
{
    Bisa::HitCollection hc;
    for (auto &&hit : hits)
    {
        if (tdcs.find(hit.tdc()) != tdcs.end())
        {
            hc.add(hit);
        }
    }
    return hc;
}

Bisa::HitCollection ScintillatorTrigger::on_coord(const Bisa::HitCollection &hits, Bisa::Coordinate coord)
{
    Bisa::HitCollection hc;
    for (auto &&hit : hits)
    {
        if (config_.coordinate(hit.tdc()) == coord)
        {
            hc.add(hit);
        }
    }
    return hc;
}
