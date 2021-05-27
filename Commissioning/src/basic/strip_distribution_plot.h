#pragma once

#include "chamber_distribution_plot.h"

class StripDistributionPlot : public ChamberDistributionPlot
{
public:
    StripDistributionPlot(const char *name, const char *title, Bisa::Experiment *experiment, const Bisa::Config &config)
     : ChamberDistributionPlot(name, title, experiment, config)
    {
        for (size_t i = 0; i < 18; i++)
        {
            ps_[i].GetXaxis()->SetTitle("Strip");
        }
        canvas()->SetTitle(title);
    }

    virtual ~StripDistributionPlot()
    {
        gStyle->SetOptStat(0);

        canvas()->SetTitle("Chamber Strip Distribution");

        canvas()->cd();
        double max_scale = 0;
        for (int i = 0; i < 18; i++)
        {
            ps_[i].Draw("HIST");
            max_scale = std::max(max_scale, ps_[i].GetMaximum());
            print(Form("tdc%d", i), name_);
            canvas()->Clear();
        }

        canvas()->Divide(3, 3);

        for (int i = 0; i < 9; i++)
        {
            configure_pad(i);
            ps_[i].SetMaximum(max_scale * 1.1);
            ps_[i].Draw("HIST");
        }
        print(Form("%s_bis7", name_));

        for (int i = 9; i < 18; i++)
        {
            configure_pad(i);
            ps_[i].Draw("HIST");
        }
        print(Form("%s_bis8", name_));
    }

    void add_hits(const Bisa::HitCollection &hits) override
    {
        for (auto &&hit : hits)
        {
            ps_[hit.tdc()].Fill(config_.strip(hit.channel()));
        }
    }
};