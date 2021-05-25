#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TString.h>
#include <TStyle.h>

class CheckPadTriggerPlot : public Bisa::Plot
{
public:
    CheckPadTriggerPlot(const char *name, const char *title, Bisa::Experiment *experiment, const Bisa::Config &config)
     : Bisa::Plot(name, title, experiment, config)
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

private:
    bool pad_trigger(const Bisa::HitCollection &hits)
    {
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
                        // if (hit1.bcid_tdc() == hit2.bcid_tdc())
                        // Same 6 MSB
                        // if ((hit1.bcid_tdc() << 1 | hit1.fine_time() >> 6) == (hit2.bcid_tdc() << 1 | hit2.fine_time() >> 6))
                        if (msb6(hit1) == msb6(hit2) || std::abs(msb6(hit1) - msb6(hit2)) == 1)
                        {
                            return true;
                        }
                        // Less than 12.5 ns apart
                        // if (std::abs(config_.time(hit1.bcid_tdc(), hit1.fine_time()) - config_.time(hit2.bcid_tdc(), hit2.fine_time())) < 12.5)
                        // {
                        //     return true;
                        // }
                    }
                }
            }
        }
        return false;
    }

    int msb6(Bisa::Hit hit)
    {
        return hit.bcid_tdc() << 1 | hit.fine_time() >> 6;
    }

    TH1F p_;
    bool trigger_;

    int event_counter_ = 0;
};