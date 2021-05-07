#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TStyle.h>

class NumLayersPlot : public Bisa::Plot
{
public:
    NumLayersPlot(const char *name, const char *title, Bisa::Experiment *experiment, const Bisa::Config &config)
     : Bisa::Plot(name, title, experiment, config)
     , p_(name, title, 4, 0, 4)
    {
        p_.GetXaxis()->SetTitle("Number of Layers Hit");
        canvas()->SetTitle(title);

        experiment_->tree()->Branch(name_, &num_layers_);
    }

    ~NumLayersPlot() 
    {
        gStyle->SetOptStat(110);

        canvas()->cd(); // switch to canvas

        p_.Draw("HIST"); // draw this plot to canvas

        print(); // save plot to pdf
    }

    int add_hits(const Bisa::HitCollection &hits)
    {
        num_layers_ = num_layers(hits);
        p_.Fill(num_layers_);
        return num_layers_;
    }

private:
    unsigned int num_layers(const Bisa::HitCollection &hits)
    {
        int eta[3] = {0, 0, 0};
        int phi[3] = {0, 0, 0};
        for (auto &&hit : hits)
        {
            if (config_.coordinate(hit.tdc()) == Bisa::ETA)
            {
                eta[config_.layer(hit.tdc())]++;
            } else {
                phi[config_.layer(hit.tdc())]++;
            }
        }

        int eta_total = 0;
        int phi_total = 0;
        for (size_t i = 0; i < 3; ++i)
        {
            eta_total += eta[i] > 0;
            phi_total += phi[i] > 0;
        }
        
        return std::max(eta_total, phi_total);
    }

    TH1F p_;
    unsigned int num_layers_;
};