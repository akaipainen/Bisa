#pragma once

#include "chamber_distribution_plot.h"

class ChannelDistributionPlot : public ChamberDistributionPlot
{
public:
    ChannelDistributionPlot(const char *name, const char *title, Bisa::Experiment *experiment)
     : ChamberDistributionPlot(name, title, experiment)
    {
        for (size_t i = 0; i < 18; i++)
        {
            ps_[i].GetXaxis()->SetTitle("Channel");
        }
        canvas()->SetTitle(title);
    }

    virtual ~ChannelDistributionPlot()
    {
        gStyle->SetOptStat(0);

        canvas()->Divide(3, 3);

        for (int i = 0; i < 18; i++)
        {
            configure_pad(i);
            ps_[i].Draw("HIST");
        }

        print();
    }

    void add_hits(const Bisa::HitCollection &hits) override
    {
        for (auto &&hit : hits)
        {
            ps_[hit.tdc()].Fill(hit.channel());
        }
    }
};