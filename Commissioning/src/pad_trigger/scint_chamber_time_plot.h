#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TSystem.h>
#include <TStyle.h>

class ScintChamberTimePlot : public Bisa::Plot
{
public:

    ScintChamberTimePlot(const char *name, const char *title, Bisa::Experiment *experiment)
     : Bisa::Plot(name, title, experiment)
     , p_(name, title, 3000, -1500, 1500)
    {
        p_.GetXaxis()->SetTitle("Average time difference [ns]");
        canvas()->SetTitle(title);
        experiment_->tree()->Branch(name_, &timing_difference_);
    }

    ~ScintChamberTimePlot()
    {
        gStyle->SetOptStat(110);

        canvas()->cd(); // switch to canvas

        p_.Draw("HIST"); // draw this plot to canvas

        print(); // save plot to pdf
    }

    void add_hits(const Bisa::HitCollection &hits)
    {
        double chamber_avg_time = 0;
        int chamber_hits = 0;
        double scint_avg_time = 0;
        int scint_hits = 0;
        for (auto &&hit : hits)
        {
            if (hit.chamber() == 8)
            {
                scint_avg_time += hit.time();
                scint_hits++;
            }
            else
            {
                chamber_avg_time += hit.time();
                chamber_hits++;
            }
        }
        chamber_avg_time /= chamber_hits;
        scint_avg_time /= scint_hits;

        p_.Fill(chamber_avg_time - scint_avg_time);
    }

private:
    TH1F p_;
    int timing_difference_;
};
