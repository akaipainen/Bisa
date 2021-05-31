#pragma once

#include <Bisa.h>

// Plots
#include "efficiency_plot.h"

#include <TTree.h>

class Efficiency : public Bisa::Experiment
{
public:
    Efficiency(const char *name, int voltage, TTree *tree, const char *output_path);

    ~Efficiency();

    void add_hits(const Bisa::HitCollection &hits);

private:
    std::set<std::pair<unsigned int, unsigned int>> ignore_strips_;
    int voltage_;

    Bisa::HitCollection select_eta(const Bisa::HitCollection &hits) const;
    Bisa::HitCollection select_phi(const Bisa::HitCollection &hits) const;

    EfficiencyPlot efficiency_top_;
    EfficiencyPlot efficiency_mid_;
    EfficiencyPlot efficiency_bot_;

    EfficiencyPlot efficiency_top_eta_;
    EfficiencyPlot efficiency_mid_eta_;
    EfficiencyPlot efficiency_bot_eta_;

    EfficiencyPlot efficiency_top_phi_;
    EfficiencyPlot efficiency_mid_phi_;
    EfficiencyPlot efficiency_bot_phi_;
};