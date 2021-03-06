#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TString.h>
#include <TStyle.h>

class TdcTimingDifferencePlot : public Bisa::Plot
{
public:
    TdcTimingDifferencePlot(const char *name, const char *title, Bisa::Experiment *experiment)
     : Bisa::Plot(name, title, experiment)
     , p_(name, title, 75 / Bisa::FINE_TIME_RESOLUTION_NS, 0, 75)
    {
        p_.GetXaxis()->SetTitle("Time Difference [ns]");
        canvas()->SetTitle(title);
        experiment_->tree()->Branch(name_, &pair_differences_);
    }

    ~TdcTimingDifferencePlot() 
    {
        gStyle->SetOptStat(110);

        canvas()->cd(); // switch to canvas

        p_.Draw("HIST"); // draw this plot to canvas

        print(); // save plot to pdf
    }

    void add_hits(const Bisa::HitCollection &hits)
    {
        pair_differences_.clear();

        for (auto hit_it1 = hits.begin(); hit_it1 != hits.end(); hit_it1++)
        {
            for (auto hit_it2 = std::next(hit_it1); hit_it2 != hits.end(); hit_it2++)
            {
                auto hit1 = *hit_it1;
                auto hit2 = *hit_it2;
                auto diff = std::abs(hit1.time() - hit2.time());
                pair_differences_.push_back(diff);
                p_.Fill(diff);
            }
        }
    }

private:

    TH1F p_;
    std::vector<double> pair_differences_;
};