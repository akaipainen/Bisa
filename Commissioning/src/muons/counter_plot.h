#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TString.h>
#include <TStyle.h>

class CounterPlot : public Bisa::Plot
{
public:
    CounterPlot(const char *name, const char *title, Bisa::Experiment *experiment)
     : Bisa::Plot(name, title, experiment)
     , p_(name, title, 1, 0, 1)
    {
        p_.GetYaxis()->SetTitle("Number of Muons");
    }

    ~CounterPlot() 
    {
        gStyle->SetOptStat(110);

        canvas()->cd(); // switch to canvas

        p_.Draw("HIST"); // draw this plot to canvas

        print(); // save plot to pdf
    }

    void increment()
    {
        p_.Fill(0);
    }

private:
    TH1F p_;
};