#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TString.h>
#include <TStyle.h>

class TDCCombinatoricsPlot : public Bisa::Plot
{
public:
    TDCCombinatoricsPlot(const char *name, const char *title, Bisa::Experiment *experiment, const Bisa::Config &config)
     : Bisa::Plot(name, title, experiment, config)
     , p_(name, title, 36, 0, 36)
    {
        p_.GetXaxis()->SetTitle("TDC Combinatorics");
        p_.SetCanExtend(TH1::kAllAxes);
        canvas()->SetTitle(title);
        experiment_->tree()->Branch(name_, &min_timing_);
    }

    ~TDCCombinatoricsPlot() 
    {
        gStyle->SetOptStat(0);
        p_.LabelsDeflate("X"); // Delete any bins that are not present
        p_.LabelsOption("av"); // sort by increasing value

        canvas()->cd(); // switch to canvas

        p_.Draw("HIST"); // draw this plot to canvas

        print(); // save plot to pdf
    }

    void add_hits(const Bisa::HitCollection &hits)
    {
        std::map<std::pair<int, int>, int> pairs;
        for (auto hit1_it = hits.begin(); hit1_it != hits.end(); hit1_it++)
        {
            for (auto hit2_it = std::next(hit1_it); hit2_it != hits.end(); hit2_it++)
            {
                if (std::abs(config_.time(hit1_it->bcid_tdc(), hit1_it->fine_time()) - config_.time(hit2_it->bcid_tdc(), hit2_it->fine_time())) < 12.5)
                {
                    int min_tdc = std::min(hit1_it->tdc(), hit2_it->tdc());
                    int max_tdc = std::max(hit1_it->tdc(), hit2_it->tdc());
                    if (pairs.find({min_tdc, max_tdc}) == pairs.end())
                    {
                        pairs[{min_tdc, max_tdc}] = 1;
                    } else {
                        pairs[{min_tdc, max_tdc}]++;
                    }
                }
            }
        }

        if (pairs.size() == 1)
        {
            p_.Fill(Form("%d-%d", pairs.begin()->first.first, pairs.begin()->first.second), 1);
        }

    }

private:
    TH1F p_;
    double min_timing_;
};