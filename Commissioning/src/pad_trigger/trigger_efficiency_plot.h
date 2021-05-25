#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TString.h>
#include <TStyle.h>

class TriggerEfficiencyPlot : public Bisa::Plot
{
public:
    TriggerEfficiencyPlot(const char *name, const char *title, Bisa::Experiment *experiment, const Bisa::Config &config)
     : Bisa::Plot(name, title, experiment, config)
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
        // float scint_time = 0;
        Bisa::HitCollection chamber_hits;
        // float chamber_time = 0;

        for (auto &&hit1 : hits)
        {
            if (config_.chamber(hit1.tdc()) == 7)
            {
                chamber_hits.add(hit1);
                // chamber_time = std::min(chamber_time, config_.time(hit1.bcid_tdc(), hit1.fine_time()));
            }
            else // bis8 = scintillator hits
            {
                scint_hits.add(hit1);
                // scint_time = std::min(scint_time, config_.time(hit1.bcid_tdc(), hit1.fine_time()));
            }
        }

        if (pad_trigger(scint_hits))
        {
            if (pad_trigger(chamber_hits))
            {
                return 1;
            }
            return 0;
        }
        return -1;
    }

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
    int trigger_;

    int probe_counter_ = 0;
};