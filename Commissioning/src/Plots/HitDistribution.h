#pragma once

#include <Bisa.h>

#include <TH1F.h>
#include <TSystem.h>
#include <TStyle.h>

class HitDistribution : public Bisa::Plot
{
public:

    HitDistribution(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 1, 1, config)
     , strip_rate_(Form("%s_strip_rate", name_), config)
     , channel_rate_(Form("%s_channel_rate", name_), config)
     , strip_count_(Form("%s_strip_count", name_), config)
     , channel_count_(Form("%s_channel_count", name_), config)
    {
        init();
    }

    virtual ~HitDistribution()
    {
        configure();
        save();
    }

    virtual void addHits(Bisa::HitCollection hits)
    {
        for (auto &&hit : hits)
        {
            strip_rate_[hit.tdc()].Fill(strip(hit));
            channel_rate_[hit.tdc()].Fill(hit.channel());
            strip_count_[hit.tdc()].Fill(strip(hit));
            channel_count_[hit.tdc()].Fill(hit.channel());
        }
    }

    void init()
    {   
        gDirectory->cd(name_);

        strip_rate_.init(32, 0, 32);
        channel_rate_.init(32, 0, 32);
        strip_count_.init(32, 0, 32);
        channel_count_.init(32, 0, 32);

        gDirectory->cd("..");
    }

    void configure()
    {
        strip_rate_.configure();
        channel_rate_.configure();
        strip_count_.configure();
        channel_count_.configure();

        strip_rate_.for_each([&] (TH1F& hist) {
            same_configure(hist);

            hist.GetXaxis()->SetTitle("Strip");
            hist.GetYaxis()->SetTitle("Rate [hz]");
        });
        channel_rate_.for_each([&] (TH1F& hist) {
            same_configure(hist);

            hist.GetXaxis()->SetTitle("Channel");
            hist.GetYaxis()->SetTitle("Rate [hz]");
        });
        strip_count_.for_each([&] (TH1F& hist) {
            same_configure(hist);

            hist.GetXaxis()->SetTitle("Strip");
            hist.GetYaxis()->SetTitle("Count");
        });
        channel_count_.for_each([&] (TH1F& hist) {
            same_configure(hist);

            hist.GetXaxis()->SetTitle("Channel");
            hist.GetYaxis()->SetTitle("Count");
        });
    }

    void save()
    {
        gStyle->SetOptStat(0);
        gStyle->SetOptFit(0);

        gSystem->mkdir("output/hit_distribution", true);

        auto props = Bisa::SummaryTdc<TH1F>::DrawProps();
        props.options = "BAR E0"; // options
        props.logy = true;

        strip_rate_.print("output/hit_distribution", props);
        channel_rate_.print("output/hit_distribution", props);
        strip_count_.print("output/hit_distribution", props);
        channel_count_.print("output/hit_distribution", props);
    }

protected:
    void same_configure(TH1F& hist)
    {
        // for (int strip = 0; strip < 32; strip++)
        // {
        //     hist.GetXaxis()->SetBinLabel(strip+1, Form("%d", strip));
        // }
        hist.GetXaxis()->SetLabelSize(0.1);

        hist.SetFillColor(16); // Bar color
        hist.SetLineColor(kBlack); // Error bar color
        hist.SetBarWidth(0.8); // Set bar width
        hist.SetBarOffset(0.1);
    }

    unsigned int strip(const Bisa::Hit& hit)
    {
        return config_.strip(hit.channel());
    }

public:
    Bisa::SummaryTdc<TH1F> strip_rate_;
    Bisa::SummaryTdc<TH1F> channel_rate_;

    Bisa::SummaryTdc<TH1F> strip_count_;
    Bisa::SummaryTdc<TH1F> channel_count_;
};
