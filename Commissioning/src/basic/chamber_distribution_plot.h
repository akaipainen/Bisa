#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TSystem.h>
#include <TStyle.h>

class ChamberDistributionPlot : public Bisa::Plot
{
public:

    ChamberDistributionPlot(const char* name, const char *title, Bisa::Experiment *experiment, const Bisa::Config& config)
     : Bisa::Plot(name, title, experiment, config)
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
        if (config_.coordinate(tdc) == Bisa::ETA && config_.layer(tdc) == 0) canvas()->cd(1);
        if (config_.coordinate(tdc) == Bisa::ETA && config_.layer(tdc) == 1) canvas()->cd(4);
        if (config_.coordinate(tdc) == Bisa::ETA && config_.layer(tdc) == 2) canvas()->cd(7);
        if (config_.coordinate(tdc) == Bisa::PHI && config_.layer(tdc) == 0 && config_.orientation(tdc) == 0) canvas()->cd(2);
        if (config_.coordinate(tdc) == Bisa::PHI && config_.layer(tdc) == 0 && config_.orientation(tdc) == 1) canvas()->cd(3);
        if (config_.coordinate(tdc) == Bisa::PHI && config_.layer(tdc) == 1 && config_.orientation(tdc) == 0) canvas()->cd(5);
        if (config_.coordinate(tdc) == Bisa::PHI && config_.layer(tdc) == 1 && config_.orientation(tdc) == 1) canvas()->cd(6);
        if (config_.coordinate(tdc) == Bisa::PHI && config_.layer(tdc) == 2 && config_.orientation(tdc) == 0) canvas()->cd(8);
        if (config_.coordinate(tdc) == Bisa::PHI && config_.layer(tdc) == 2 && config_.orientation(tdc) == 1) canvas()->cd(9);

        // Configure margins                                        L    R    T    B
        if (config_.coordinate(tdc) == Bisa::ETA)  gPad->SetMargin(0.1, 0.1, 0.1, 0.1);
        else if (config_.orientation(tdc) == 1)    gPad->SetMargin(0.0, 0.1, 0.1, 0.1);
        else                                       gPad->SetMargin(0.1, 0.0, 0.1, 0.1);
    }

    std::vector<TH1F> ps_;
};
