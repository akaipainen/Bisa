#pragma once

#include <Bisa.h>

#include <TH1.h>
#include <TSystem.h>
#include <TStyle.h>

class AdjacentHitsDistribution : public Bisa::Plot
{
public:
    AdjacentHitsDistribution(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 1, 1, config)
     , strip_(Form("%s_strip", name_), config)
    {
        init();
    }

    ~AdjacentHitsDistribution()
    {
        configure();
        save();
    }

    void addHits(Bisa::HitCollection hits)
    {
        for (auto &&hit : hits)
        {
            strip_[hit.tdc()].Fill(strip(hit), 1.0/hits.size());
        }
    }

    void init()
    {
        gDirectory->cd(name_);
        strip_.init(32, 0, 32);
        gDirectory->cd("..");
    }

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

    void configure()
    {
        strip_.configure();

        strip_.for_each([&] (TH1F& hist) {
            same_configure(hist);

            hist.GetXaxis()->SetTitle("Strip");
            hist.GetYaxis()->SetTitle("Width");

            hist.SetMinimum(1);
        });
    }

    void save()
    {
        gStyle->SetOptStat(0);
        gStyle->SetOptFit(0);

        gSystem->mkdir("output/strip_mapping", true);

        auto props = Bisa::SummaryTdc<TH1F>::DrawProps();
        props.options = "BAR E0";
        props.logy = true;

        strip_.print("output/strip_mapping", props);
    }

private:
    unsigned int strip(const Bisa::Hit& hit)
    {
        return config_.strip(hit.channel());
    }

private:
    Bisa::SummaryTdc<TH1F> strip_;
};