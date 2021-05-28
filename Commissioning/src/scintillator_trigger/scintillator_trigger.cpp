#include "scintillator_trigger.h"

ScintillatorTrigger::ScintillatorTrigger(const char *name, TTree *tree, const char *output_path)
 : Bisa::Experiment(name, tree, output_path)
//  , check_trigger_("check_pad_trigger", "Reconstructed PAD Triggers", this)
 , scint_fpga_timing_("scint_fpga_timing_jitter", "Maximum FPGA Timing Jitter between Scintillator Trigger Hits", this)
 , chamber_fpga_timing_("chamber_fpga_timing_jitter", "Maximum FPGA Timing Jitter between Chamber Hits", this)
 , check_pad_trigger_("check_pad_trigger_w_scint", "Percentage of PAD Triggers with scintillator trigger", this)
 , check_pad_trigger_eta_("check_pad_trigger_w_scint_eta", "Percentage of eta PAD triggers with scintillator trigger", this)
 , check_pad_trigger_phi_("check_pad_trigger_w_scint_phi", "Percentage of phi PAD triggers with scintillator trigger", this)
{
}

ScintillatorTrigger::~ScintillatorTrigger()
{
}

void ScintillatorTrigger::add_hits(const Bisa::HitCollection &hits)
{
    Bisa::HitCollection scint_hits = on_chamber(hits, 1); // 1=scintillator
    Bisa::HitCollection chamber_hits = hits - scint_hits;

    scint_fpga_timing_.add_hits(scint_hits);
    chamber_fpga_timing_.add_hits(chamber_hits);

    check_pad_trigger_.add_hits(chamber_hits);

    Bisa::HitCollection eta_hits = on_coord(chamber_hits, Bisa::ETA);
    check_pad_trigger_eta_.add_hits(eta_hits);
    
    Bisa::HitCollection phi_hits = on_coord(chamber_hits, Bisa::PHI);
    check_pad_trigger_phi_.add_hits(phi_hits);
}

Bisa::HitCollection ScintillatorTrigger::on_coord(const Bisa::HitCollection &hits, Bisa::Coordinate coord)
{
    Bisa::HitCollection hc;
    for (auto &&hit : hits)
    {
        if (hit.coordinate() == coord)
        {
            hc.add(hit);
        }
    }
    return hc;
}

Bisa::HitCollection ScintillatorTrigger::on_chamber(const Bisa::HitCollection &hits, unsigned int chamber)
{
    Bisa::HitCollection hc;
    for (auto &&hit : hits)
    {
        if (hit.chamber() == chamber)
        {
            hc.add(hit);
        }
    }
    return hc;
}
