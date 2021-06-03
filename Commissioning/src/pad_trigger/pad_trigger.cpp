#include "pad_trigger.h"

PadTrigger::PadTrigger(const char *name, TTree *tree, const char *output_path)
 : Bisa::Experiment(name, tree, output_path)
 , check_trigger_("check_pad_trigger", "Reconstructed PAD Triggers", this)
 , num_layers_("num_layers", "Number of Layers Hit", this)
 , min_timing_("min_timing", "Minimum Timing for Pair of Hits on Different Layers", this)
 , tdc_min_timing_("tdc_min_timing", "Minimum Timing for Pair of Hits on Different Layers", this)
//  , min_timing_2_layers_("min_timing_2_layers", "Minimum Timing for Pair of Hits on Different Layers (2 Layers Hit)", this)
//  , min_timing_3_layers_("min_timing_3_layers", "Minimum Timing for Pair of Hits on Different Layers (3 Layers Hit)", this)
//  , tdc_combos_("tdc_combo", "Hit TDC Combinations for Minimum Timings Greater than 12.5ns", this)
//  , tdc_combos_2_hits_("tdc_combo_2_hits", "Hit TDC Combinations for Minimum Timings Greater than 12.5ns (2 Hits)", this)
 , pad_trigger_efficiency_("trigger_efficiecy", "PAD Trigger Efficiency", this)
 , clean_muon_pad_trigger_efficiency_("clean_muon_trigger_efficiecy", "Clean Muon PAD Trigger Efficiency", this)
 , scint_chamber_time_plot_("scint_chamber_time_difference", "Scintillator-Chamber Average Time Difference", this)
{
}

PadTrigger::~PadTrigger()
{
}

void PadTrigger::add_hits(const Bisa::HitCollection &hits)
{
    bool pad_trigger_check = check_trigger_.add_hits(hits);
    int num_layers = num_layers_.add_hits(hits);
    double min_timing = min_timing_.add_hits(hits);
    tdc_min_timing_.add_hits(hits);
    int nsc = pad_trigger_efficiency_.add_hits(hits);
    clean_muon_pad_trigger_efficiency_.add_hits(hits);

    if (nsc == 1)
    {
        scint_chamber_time_plot_.add_hits(hits);
    }
    
    // if (num_layers == 2)
    // {
    //     min_timing_2_layers_.add_hits(hits);
    // }
    // else if (num_layers == 3)
    // {
    //     min_timing_3_layers_.add_hits(hits);
    // }

    // if (min_timing > 12.5)
    // {
    //     tdc_combos_.add_hits(hits);
    // }

    // if (hits.size() == 2)
    // {
    //     tdc_combos_2_hits_.add_hits(hits);
    // }
}
