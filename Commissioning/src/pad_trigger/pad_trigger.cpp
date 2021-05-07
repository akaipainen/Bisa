#include "pad_trigger.h"

PadTrigger::PadTrigger(const char *name, TTree *tree, const Bisa::Config &config)
 : Bisa::Experiment(name, tree, config)
 , check_trigger_("check_pad_trigger", "Reconstructed PAD Triggers", this, config)
 , num_layers_("num_layers", "Number of Layers Hit", this, config)
 , min_timing_("min_timing", "Minimum Timing for Pair of Hits on Different Layers", this, config)
 , min_timing_2_layers_("min_timing_2_layers", "Minimum Timing for Pair of Hits on Different Layers (2 Layers Hit)", this, config)
 , min_timing_3_layers_("min_timing_3_layers", "Minimum Timing for Pair of Hits on Different Layers (3 Layers Hit)", this, config)
 , tdc_combos_("tdc_combo", "Hit TDC Combinations for Minimum Timings Greater than 12.5ns", this, config)
 , tdc_combos_2_hits_("tdc_combo_2_hits", "Hit TDC Combinations for Minimum Timings Greater than 12.5ns (2 Hits)", this, config)
{
}

PadTrigger::~PadTrigger()
{
}

void PadTrigger::add_hits(const Bisa::HitCollection &hits)
{
    check_trigger_.add_hits(hits);
    int num_layers = num_layers_.add_hits(hits);
    double min_timing = min_timing_.add_hits(hits);
    
    if (num_layers == 2)
    {
        min_timing_2_layers_.add_hits(hits);
    }
    else if (num_layers == 3)
    {
        min_timing_3_layers_.add_hits(hits);
    }

    if (min_timing > 12.5)
    {
        tdc_combos_.add_hits(hits);
    }

    if (hits.size() == 2)
    {
        tdc_combos_2_hits_.add_hits(hits);
    }
}
