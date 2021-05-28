#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TSystem.h>
#include <TStyle.h>

class FPGATimingPlot : public Bisa::Plot
{
public:

    FPGATimingPlot(const char *name, const char *title, Bisa::Experiment *experiment)
     : Bisa::Plot(name, title, experiment)
     , p_(name, title, 8, 0, 8)
    {
        p_.GetXaxis()->SetTitle("Time difference [ns]");
        canvas()->SetTitle(title);
        experiment_->tree()->Branch(name_, &timing_difference_);
    }

    ~FPGATimingPlot()
    {
        gStyle->SetOptStat(110);

        canvas()->cd(); // switch to canvas

        p_.Draw("HIST"); // draw this plot to canvas

        print(); // save plot to pdf
    }

    void add_hits(const Bisa::HitCollection &hits)
    {
        timing_difference_ = -1;
        auto hit1_it = hits.begin();
        for (; hit1_it != hits.end(); hit1_it++)
        {
            auto hit2_it = next(hit1_it);
            for (; hit2_it != hits.end(); hit2_it++)
            {
                auto fpga1 = hit1_it->bcid_fpga();
                auto fpga2 = hit2_it->bcid_fpga();
                timing_difference_ = std::max(timing_difference_, (int) (fpga1 > fpga2 ? fpga1 - fpga2 : fpga2 - fpga1));
            }
        }
        p_.Fill(timing_difference_);
    }

private:
    TH1F p_;
    int timing_difference_;
};
