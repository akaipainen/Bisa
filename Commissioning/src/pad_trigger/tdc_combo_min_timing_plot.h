#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TString.h>
#include <TStyle.h>

class TdcComboMinTimingPlot : public Bisa::Plot
{
public:
    TdcComboMinTimingPlot(const char *name, const char *title, Bisa::Experiment *experiment)
     : Bisa::Plot(name, title, experiment)
    {
        for (int i = 0; i < 18; i++)
        {
            for (int j = i; j < 18; j++)
            {
                if (Bisa::config.chamber(i) == Bisa::config.chamber(j) &&
                    Bisa::config.coordinate(i) == Bisa::config.coordinate(j) &&
                    Bisa::config.layer(i) != Bisa::config.layer(j))
                {
                    tdc_combo_.emplace(std::piecewise_construct,
                                       std::make_tuple(i, j),
                                       std::make_tuple(Form("%s_%d_%d", name, i, j), title, 75 / Bisa::FINE_TIME_RESOLUTION_NS, 0, 75));
                    tdc_combo_[{i, j}].GetXaxis()->SetTitle("Time Difference [ns]");
                }
            }
        }
        
        canvas()->SetTitle(title);
        // experiment_->tree()->Branch(name_, &min_timing_);
    }

    ~TdcComboMinTimingPlot() 
    {
        gStyle->SetOptStat(110);

        canvas()->cd(); // switch to canvas
        for (auto &&p : tdc_combo_)
        {
            p.second.Draw("HIST"); // draw this plot to canvas
            print(Form("%s_%d_%d", name_, p.first.first, p.first.second), name_); // save plot to pdf
            canvas()->Clear();
        }
    }

    double add_hits(const Bisa::HitCollection &hits)
    {
        unsigned int tdc1 = 0;
        unsigned int tdc2 = 0;
        double min_diff = 10000;
        for (auto &&hit1 : hits)
        {
            for (auto &&hit2 : hits)
            {
                // Same chamber
                if (hit1.chamber() == hit2.chamber())
                {
                    // Same coordinate
                    if (hit1.coordinate() == hit2.coordinate())
                    {
                        // Different layer (implicitly different TDC)
                        if (hit1.layer() != hit2.layer())
                        {
                            auto diff = std::abs(hit1.time() - hit2.time());
                            if (diff < min_diff)
                            {
                                min_diff = diff;
                                tdc1 = hit1.tdc();
                                tdc2 = hit2.tdc();
                            }
                        }
                    }
                }
            }
        }

        if (min_diff < 10000)
        {
            if (tdc1 > tdc2) std::swap(tdc1, tdc2);
            tdc_combo_[{tdc1, tdc2}].Fill(min_diff);
        }
        return min_diff;
    }

private:
    std::map<std::pair<unsigned int, unsigned int>, TH1F> tdc_combo_;
};