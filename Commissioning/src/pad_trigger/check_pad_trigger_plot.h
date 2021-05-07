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

    void add_hits(const Bisa::HitCollection &hits)
    {
        trigger_ = pad_trigger(hits);
        p_.Fill(trigger_);
        ++event_counter_;
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
                        // Less than 12.5 ns apart
                        if (std::abs(config_.time(hit1.bcid_tdc(), hit1.fine_time()) - config_.time(hit2.bcid_tdc(), hit2.fine_time())) < 12.5)
                        {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    TH1F p_;
    bool trigger_;

    int event_counter_ = 0;
};