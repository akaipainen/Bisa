#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TSystem.h>
#include <TStyle.h>

class ChamberDistributionPlot : public Bisa::Plot
{
public:

    ChamberDistributionPlot(const char* name, const char *title, Bisa::Experiment *experiment)
     : Bisa::Plot(name, title, experiment)
    {
        for (int i = 0; i < 18; i++)
        {
            ps_.push_back(TH1F(Form("%s_tdc%d", name, i), Form("%s TDC %d", title, i), 32, 0, 32));
        }
    }

    virtual ~ChamberDistributionPlot()
    {
    }

    virtual void add_hits(const Bisa::HitCollection &hits) = 0;

protected:
    void configure_pad(int tdc)
    {
        // Set the correct pad
        if (Bisa::config.coordinate(tdc) == Bisa::ETA && Bisa::config.layer(tdc) == 0)                                       canvas()->cd(1);
        if (Bisa::config.coordinate(tdc) == Bisa::ETA && Bisa::config.layer(tdc) == 1)                                       canvas()->cd(4);
        if (Bisa::config.coordinate(tdc) == Bisa::ETA && Bisa::config.layer(tdc) == 2)                                       canvas()->cd(7);
        if (Bisa::config.coordinate(tdc) == Bisa::PHI && Bisa::config.layer(tdc) == 0 && Bisa::config.orientation(tdc) == 0) canvas()->cd(2);
        if (Bisa::config.coordinate(tdc) == Bisa::PHI && Bisa::config.layer(tdc) == 0 && Bisa::config.orientation(tdc) == 1) canvas()->cd(3);
        if (Bisa::config.coordinate(tdc) == Bisa::PHI && Bisa::config.layer(tdc) == 1 && Bisa::config.orientation(tdc) == 0) canvas()->cd(5);
        if (Bisa::config.coordinate(tdc) == Bisa::PHI && Bisa::config.layer(tdc) == 1 && Bisa::config.orientation(tdc) == 1) canvas()->cd(6);
        if (Bisa::config.coordinate(tdc) == Bisa::PHI && Bisa::config.layer(tdc) == 2 && Bisa::config.orientation(tdc) == 0) canvas()->cd(8);
        if (Bisa::config.coordinate(tdc) == Bisa::PHI && Bisa::config.layer(tdc) == 2 && Bisa::config.orientation(tdc) == 1) canvas()->cd(9);

        // Configure margins                                            L    R    T    B
        if (Bisa::config.coordinate(tdc) == Bisa::ETA) gPad->SetMargin(0.1, 0.1, 0.1, 0.1);
        else if (Bisa::config.orientation(tdc) == 1)   gPad->SetMargin(1.0, 0.1, 0.1, 0.1);
        else                                           gPad->SetMargin(0.1, 1.0, 0.1, 0.1);
    }

    std::vector<TH1F> ps_;
};
