#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TString.h>
#include <TStyle.h>

class CheckPadTriggerPlot : public Bisa::Plot
{
public:
    CheckPadTriggerPlot(const char *name, const char *title, Bisa::Experiment *experiment)
     : Bisa::Plot(name, title, experiment)
     , p_(name, title, 2, 0, 2)
    {
        p_.GetXaxis()->SetTitle("Triggers [1 = Trigger; 0 = No Trigger]");
        p_.GetYaxis()->SetTitle("Percentage [%]");
        experiment_->tree()->Branch(name_, &trigger_);
    }

    ~CheckPadTriggerPlot() 
    {
        gStyle->SetOptStat(110);

        p_.Scale(1.0 / event_counter_); // Get percentage for efficiency

        canvas()->cd(); // switch to canvas

        p_.Draw("HIST"); // draw this plot to canvas

        print(); // save plot to pdf
    }

    bool add_hits(const Bisa::HitCollection &hits)
    {
        trigger_ = pad_trigger(hits);
        p_.Fill(trigger_);
        ++event_counter_;
        return trigger_;
    }

    static bool pad_trigger(const Bisa::HitCollection &hits)
    {
        for (auto &&hit1 : hits)
        {
            for (auto &&hit2 : hits)
            {
                // Same chamber
                if (hit1.chamber() == hit2.chamber())
                {
                    // Same coordinate
                    if (hit1.coordinate() == hit1.coordinate())
                    {
                        // Different layer (implicitly different TDC)
                        if (hit1.layer() != hit2.layer())
                        {
                            // Same 6 MSB or 1 BCID apart
                            if (msb6(hit1) == msb6(hit2) || std::abs(msb6(hit1) - msb6(hit2)) == 1)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

private:
    static int msb6(Bisa::Hit hit)
    {
        return hit.bcid_tdc() << 1 | hit.fine_time() >> 6;
    }

    TH1F p_;
    bool trigger_;

    int event_counter_ = 0;
};