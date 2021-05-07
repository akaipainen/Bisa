#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TString.h>
#include <TStyle.h>

class TdcFpgaLatencyPlot : public Bisa::Plot
{
public:
    TdcFpgaLatencyPlot(const char *name, const char *title, Bisa::Experiment *experiment, const Bisa::Config &config)
     : Bisa::Plot(name, title, experiment, config)
     , rollover(1 << (config_.pairmode() ? 5 : 12))
     , p_(name, title, rollover, 0, rollover)
    {
        p_.GetXaxis()->SetTitle("Latency [clock ticks]");
        canvas()->SetTitle(title);
        experiment_->tree()->Branch(name_, &tdc_fpga_latency_);
    }

    ~TdcFpgaLatencyPlot() 
    {
        gStyle->SetOptStat(110);

        canvas()->cd(); // switch to canvas

        p_.Draw("HIST"); // draw this plot to canvas

        print(); // save plot to pdf
    }

    void add_hits(const Bisa::HitCollection &hits)
    {
        for (auto &&hit : hits)
        {
            auto latency = hit.bcid_fpga() % rollover - hit.bcid_tdc();
            tdc_fpga_latency_.push_back(latency);
            p_.Fill(latency);
        }
    }

private:
    int rollover;
    TH1F p_;
    std::vector<double> tdc_fpga_latency_;
};