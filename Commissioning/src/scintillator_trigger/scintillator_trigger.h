#pragma once

#include <Bisa.h>
#include "../plots.h"

// Plots
#include "fpga_timing_plot.h"

#include <TTree.h>

class ScintillatorTrigger : public Bisa::Experiment
{
public:
    ScintillatorTrigger(const char *name, TTree *tree, const Bisa::Config &config);

    ~ScintillatorTrigger();

    void add_hits(const Bisa::HitCollection &hits);

private:
    Bisa::HitCollection on_tdcs(const Bisa::HitCollection &hits, const std::set<unsigned int> &tdcs);
    
    Bisa::HitCollection on_coord(const Bisa::HitCollection &hits, Bisa::Coordinate coord);

    FPGATimingPlot scint_fpga_timing_;
    FPGATimingPlot chamber_fpga_timing_;

    CheckPadTriggerPlot check_pad_trigger_;
    CheckPadTriggerPlot check_pad_trigger_eta_;
    CheckPadTriggerPlot check_pad_trigger_phi_;
};