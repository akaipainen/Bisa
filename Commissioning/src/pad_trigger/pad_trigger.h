#pragma once

#include <Bisa.h>

// Plots
#include "check_pad_trigger_plot.h"
#include "num_layers_plot.h"
#include "min_timing_plot.h"
#include "tdc_combo_min_timing_plot.h"
#include "tdc_combinatorics_plot.h"
#include "trigger_efficiency_plot.h"
#include "scint_chamber_time_plot.h"
#include "clean_muon_trigger_efficiency_plot.h"

#include <TTree.h>

class PadTrigger : public Bisa::Experiment
{
public:
    PadTrigger(const char *name, TTree *tree, const char *output_path);

    ~PadTrigger();

    void add_hits(const Bisa::HitCollection &hits);

private:
    CheckPadTriggerPlot check_trigger_;
    NumLayersPlot num_layers_;

    MinTimingPlot min_timing_;
    TdcComboMinTimingPlot tdc_min_timing_;
    // MinTimingPlot min_timing_2_layers_;
    // MinTimingPlot min_timing_3_layers_;

    // TDCCombinatoricsPlot tdc_combos_;
    // TDCCombinatoricsPlot tdc_combos_2_hits_;

    TriggerEfficiencyPlot pad_trigger_efficiency_;
    CleanMuonTriggerEfficiencyPlot clean_muon_pad_trigger_efficiency_;

    ScintChamberTimePlot scint_chamber_time_plot_;
};