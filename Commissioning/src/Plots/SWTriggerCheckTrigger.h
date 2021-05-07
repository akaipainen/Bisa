#pragma once

#include <Bisa.h>

#include <TH2F.h>
#include <TProfile.h>
#include <TSystem.h>
#include <TStyle.h>

class SwTriggerCheckTrigger : Bisa::Plot
{
public:

    SwTriggerCheckTrigger(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 1, 1, config)
     , all_(Form("%s_all", name_), config)
    {
        init();
    }

    ~SwTriggerCheckTrigger()
    {
        configure();
        save();
    }

    void addHits(Bisa::HitCollection hits)
    {
        for (auto &&hit1 : hits)
        {
            for (auto &&hit2 : hits)
            {
                if (config_.coordinate(hit1.tdc()) == config_.coordinate(hit2.tdc())) // Same coord
                {
                    if (config_.layer(hit1.tdc()) != config_.layer(hit2.tdc())) // Different layer
                    {
                        if (hits_time_apart(hit1, hit2) < 13) // Time apart
                        {
                            all_().Fill(1);
                            event_counter_++;
                            return;
                        }
                    }
                }
            }
        }
        all_().Fill(0);
        event_counter_++;
    }

    void addTrigger(bool value)
    {
        all_().Fill(value);
        event_counter_++;
    }

    void init()
    {   
        gDirectory->cd(Form("/%s", name_));

        all_.init(2, 0, 2);
        
        gDirectory->cd("..");
    }

    void same_configure(TH1F& hist)
    {
        hist.Scale(1.0 / event_counter_);
        hist.GetXaxis()->SetTitle("Trigger checked (0=no, 1=yes)");
        hist.GetYaxis()->SetTitle("Number of Triggers");
    }

    void configure()
    {
        all_.configure();
        all_.apply([&] (TH1F& hist) {
            same_configure(hist);
        });
    }

    void save()
    {
        gStyle->SetOptFit(0);
        gStyle->SetOptStat(0);

        gSystem->mkdir("output/sw_trigger", true);

        auto props = Bisa::SummaryAgg<TH1F>::DrawProps();
        props.logy = true;

        all_.print("output/sw_trigger", props);
    }
private:
    unsigned int hits_time_apart(const Bisa::Hit &hit1, const Bisa::Hit &hit2)
    {
        return abs(config_.time(hit1.bcid_tdc(), hit1.fine_time()) - config_.time(hit2.bcid_tdc(), hit1.fine_time()));
    }

private:
    Bisa::SummaryAgg<TH1F> all_;

    unsigned int event_counter_ = 0;

};
