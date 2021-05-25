#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TString.h>
#include <TStyle.h>

class EfficiencyPlot : public Bisa::Plot
{
public:
    EfficiencyPlot(const char *name, const char *title, int voltage, int layer, Bisa::Experiment *experiment, const Bisa::Config &config)
     : Bisa::Plot(name, title, experiment, config)
     , p_(Form("%s_%d", name, voltage), title, 15, 4500, 6000)
     , voltage_(voltage)
     , layer_(layer)
    {
        p_.GetXaxis()->SetTitle("Voltage [V]");
        p_.GetYaxis()->SetTitle("Efficiency [%]");

        experiment_->tree()->Branch(Form("%s_%d", name, voltage), &trigger_);
    }

    ~EfficiencyPlot() 
    {
        // BA_INFO(1.0 / event_counter_);
        gStyle->SetOptStat(0);
        p_.Scale(100.0 / probe_counter_); // Get percentage for efficiency
        canvas()->cd(); // switch to canvas
        p_.Draw("HIST"); // draw this plot to canvas
        print(); // save plot to pdf
    }

    void add_hits(const Bisa::HitCollection &hits)
    {
        trigger_ = test_probe(hits);
        if (trigger_ >= 0) probe_counter_++;
        if (trigger_ == 1) p_.Fill(voltage_);
    }

private:
    // Return 1 if test found, 0 if no test, -1 if no probe
    int test_probe(const Bisa::HitCollection &hits)
    {
        unsigned int probe_layer_1 = layer_ != 0 ? 0 : 1;
        unsigned int probe_layer_2 = layer_ != 2 ? 2 : 1;
        bool probe_found = false;
        for (auto &&hit1 : hits)
        {
            if (config_.layer(hit1.tdc()) == probe_layer_1)
            {
                for (auto &&hit2 : hits)
                {
                    if (config_.layer(hit2.tdc()) == probe_layer_2 &&
                        config_.chamber(hit1.tdc()) == config_.chamber(hit2.tdc()) &&
                        config_.coordinate(hit1.tdc()) == config_.coordinate(hit2.tdc()))
                    {
                        // Spatial+Timing requirement for probe hits
                        if (time_apart(hit1, hit2) < 2)
                        {
                            probe_found = true;
                            for (auto &&hit3 : hits) // Search for test hit
                            {
                                if (config_.layer(hit3.tdc()) == layer_ &&
                                    config_.chamber(hit3.tdc()) == config_.chamber(hit1.tdc()) &&
                                    config_.coordinate(hit3.tdc()) == config_.coordinate(hit1.tdc()))
                                {
                                    // Spatial+Timing requirement for test hit
                                    if (time_apart(hit3, hit1) < 2 || time_apart(hit3, hit2) < 2)
                                    {
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                }   
            }
        }
        if (probe_found) return 0;
        return -1;
    }

    unsigned int distance(const Bisa::Hit &hit1, const Bisa::Hit &hit2)
    {
        return std::abs((int) hit1.strip() - (int) hit2.strip());
    }

    double time_apart(const Bisa::Hit &hit1, const Bisa::Hit &hit2)
    {
        return std::abs(config_.time(hit1.bcid_tdc(), hit1.fine_time()) - config_.time(hit2.bcid_tdc(), hit2.fine_time()));
    }

    TH1F p_;
    int voltage_;
    int layer_;
    int trigger_;

    int probe_counter_ = 0;
};