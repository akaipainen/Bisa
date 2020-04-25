#pragma once

#include <Bisa.h>

#include <TH1.h>
#include <TSystem.h>
#include <TStyle.h>

class AdjacentHitsDistribution : public Bisa::Plot
{
public:
    AdjacentHitsDistribution(const char* name)
     : Bisa::Plot(name, 1, 1)
     , strip_(Form("%s_strip", name_))
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
            strip_[hit.tdc()].Fill(strip(hit));
        }
    }

    void init()
    {
        strip_.init(32, 0, 32);
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
        });
    }

    void save()
    {
        gStyle->SetOptStat(0);
        gStyle->SetOptFit(0);

        gSystem->mkdir("output/strip_mapping_distribution", true);

        auto props = Bisa::SummaryTdc<TH1F>::DrawProps();
        props.options = "BAR E0";

        strip_.draw(canvas_, props);
        canvas_->Print("output/strip_mapping_distribution/strip_.pdf");
        canvas_->Clear();
    }

private:
    unsigned int strip(const Bisa::Hit& hit)
    {
        return strip_mapping_[hit.channel()];
    }

private:
    Bisa::SummaryTdc<TH1F> strip_;
};