#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TString.h>
#include <TStyle.h>

#include "check_pad_trigger_plot.h"

class TriggerEfficiencyPlot : public Bisa::Plot
{
public:
    TriggerEfficiencyPlot(const char *name, const char *title, Bisa::Experiment *experiment)
     : Bisa::Plot(name, title, experiment)
     , p_(name, title, 2, 0, 2)
    {
        p_.GetXaxis()->SetTitle("Triggers [1 = Trigger; 0 = No Trigger]");
        p_.GetYaxis()->SetTitle("Percentage [%]");

        experiment_->tree()->Branch(name, &trigger_);
    }

    ~TriggerEfficiencyPlot() 
    {
        // BA_INFO(1.0 / event_counter_);
        gStyle->SetOptStat(110);
        p_.Scale(100.0 / probe_counter_); // Get percentage for efficiency
        canvas()->cd(); // switch to canvas
        p_.Draw("HIST"); // draw this plot to canvas
        print(); // save plot to pdf
    }

    int add_hits(const Bisa::HitCollection &hits)
    {
        trigger_ = test_probe(hits);
        if (trigger_ >= 0)
        {
            probe_counter_++;
            p_.Fill(trigger_);
        }
        return trigger_;
    }

private:
    // Return 1 if chamber trigger found, 0 if no chamber, -1 if no scintillator
    int test_probe(const Bisa::HitCollection &hits)
    {
        Bisa::HitCollection scint_hits;
        Bisa::HitCollection chamber_hits;

        for (auto &&hit1 : hits)
        {
            if (hit1.chamber() == 1) // scintillator
            {
                scint_hits.add(hit1);
            }
            else // chamber hits
            {
                chamber_hits.add(hit1);
            }
        }

        if (scint_hits.size() > 0)
        {
            if (CheckPadTriggerPlot::pad_trigger(chamber_hits))
            {
                return 1;
            }
            return 0;
        }
        return -1;
    }

    TH1F p_;
    int trigger_;

    int probe_counter_ = 0;
};