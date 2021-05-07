#pragma once

#include <Bisa.h>

// Plots
#include "check_pad_trigger_plot.h"
#include "num_layers_plot.h"
#include "min_timing_plot.h"
#include "tdc_combinatorics_plot.h"

#include <TTree.h>

class PadTrigger : public Bisa::Experiment
{
public:
    PadTrigger(const char *name, TTree *tree, const Bisa::Config &config);

    ~PadTrigger();

    void add_hits(const Bisa::HitCollection &hits);

private:
    CheckPadTriggerPlot check_trigger_;
    NumLayersPlot num_layers_;

    MinTimingPlot min_timing_;
    MinTimingPlot min_timing_2_layers_;
    MinTimingPlot min_timing_3_layers_;

    TDCCombinatoricsPlot tdc_combos_;
    TDCCombinatoricsPlot tdc_combos_2_hits_;
};