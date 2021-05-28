#pragma once

#include <Bisa.h>

#include "../Selector.h"

// Plots
#include "strip_distribution_plot.h"
#include "channel_distribution_plot.h"
#include "tdc_timing_difference_plot.h"
#include "tdc_fpga_latency_plot.h"

#include <TTree.h>

class Basic : public Bisa::Experiment
{
public:
    Basic(const char *name, TTree *tree, const char *output_path);

    ~Basic();

    void add_hits(const Bisa::HitCollection &hits);

private:
    StripDistributionPlot muon_strip_dist_;

    StripDistributionPlot strip_dist_;
    ChannelDistributionPlot channel_dist_;
    TdcTimingDifferencePlot timing_diff_;
    TdcFpgaLatencyPlot tdc_fpga_latency_;

    unsigned int num_hits_;
    unsigned int trigger_id_;
    std::vector<unsigned int> tdc_;
    std::vector<unsigned int> strip_;
    std::vector<unsigned int> channel_;
    std::vector<unsigned int> width_;
    std::vector<unsigned int> fine_time_;
    std::vector<unsigned int> bcid_tdc_;
    std::vector<unsigned int> bcid_fpga_;
    std::vector<unsigned int> felix_counter_;
};