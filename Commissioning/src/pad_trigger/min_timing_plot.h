#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TString.h>
#include <TStyle.h>

class MinTimingPlot : public Bisa::Plot
{
public:
    MinTimingPlot(const char *name, const char *title, Bisa::Experiment *experiment)
     : Bisa::Plot(name, title, experiment)
     , p_(name, title, 75 / Bisa::FINE_TIME_RESOLUTION_NS, 0, 75)
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
                // Same chamber
                if (hit1.chamber() == hit2.chamber())
                {
                    // Same coordinate
                    if (hit1.coordinate() == hit2.coordinate())
                    {
                        // Different layer (implicitly different TDC)
                        if (hit1.layer() != hit2.layer())
                        {
                            auto diff = std::abs(hit1.time() - hit2.time());
                            if (diff < min_diff)
                            {
                                min_diff = diff;
                            }
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