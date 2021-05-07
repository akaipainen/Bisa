#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TString.h>
#include <TStyle.h>

class MinTimingPlot : public Bisa::Plot
{
public:
    MinTimingPlot(const char *name, const char *title, Bisa::Experiment *experiment, const Bisa::Config &config)
     : Bisa::Plot(name, title, experiment, config)
     , p_(name, title, 75 / config_.fine_time_resolution(), 0, 75)
    {
        p_.GetXaxis()->SetTitle("Time Difference [ns]");
        canvas()->SetTitle(title);
        experiment_->tree()->Branch(name_, &min_timing_);
    }

    ~MinTimingPlot() 
    {
        gStyle->SetOptStat(110);

        canvas()->cd(); // switch to canvas

        p_.Draw("HIST"); // draw this plot to canvas

        print(); // save plot to pdf
    }

    double add_hits(const Bisa::HitCollection &hits)
    {
        min_timing_ = min_timing(hits);
        p_.Fill(min_timing_);
        return min_timing_;
    }

private:
    double min_timing(const Bisa::HitCollection &hits)
    {
        double min_diff = 10000;
        for (auto &&hit1 : hits)
        {
            for (auto &&hit2 : hits)
            {
                // Same coordinate
                if (config_.coordinate(hit1.tdc()) == config_.coordinate(hit2.tdc()))
                {
                    // Different layer (implicitly different TDC)
                    if (config_.layer(hit1.tdc()) != config_.layer(hit2.tdc()))
                    {
                        auto diff = std::abs(config_.time(hit1.bcid_tdc(), hit1.fine_time()) - config_.time(hit2.bcid_tdc(), hit2.fine_time()));
                        if (diff < min_diff)
                        {
                            min_diff = diff;
                        }
                    }
                }
            }
        }
        return min_diff;
    }

    TH1F p_;
    double min_timing_;
};