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
     , strip_rate_(Form("%s_strip_rate", name_))
     , channel_rate_(Form("%s_channel_rate", name_))
     , strip_count_(Form("%s_strip_count", name_))
     , channel_count_(Form("%s_channel_count", name_))
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
        strip_rate_.init(32, 0, 32);
        channel_rate_.init(32, 0, 32);
        strip_count_.init(32, 0, 32);
        channel_count_.init(32, 0, 32);
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

        Bisa::SummaryTdc<TH1F>::DrawProps props = {
            true, // divide
            false, // logy
            false, // logz
            "BAR E0" // options
        };

        strip_rate_.draw(canvas_, props);
        canvas_->Print(Form("output/hit_distribution/%s_strip_rate.pdf", name_));
        canvas_->Clear();

        channel_rate_.draw(canvas_, props);
        canvas_->Print(Form("output/hit_distribution/%s_channel_rate.pdf", name_));
        canvas_->Clear();

        strip_count_.draw(canvas_, props);
        canvas_->Print(Form("output/hit_distribution/%s_strip_count.pdf", name_));
        canvas_->Clear();
        
        channel_count_.draw(canvas_, props);
        canvas_->Print(Form("output/hit_distribution/%s_channel_count.pdf", name_));
        canvas_->Clear();
    }

protected:
    void same_configure(TH1F& hist)
    {
        for (int strip = 0; strip < 32; strip++)
        {
            hist.GetXaxis()->SetBinLabel(strip+1, Form("%d", strip));
        }
        hist.GetXaxis()->SetLabelSize(0.04);

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
