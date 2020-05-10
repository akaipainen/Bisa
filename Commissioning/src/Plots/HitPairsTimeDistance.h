#pragma once

#include <Bisa.h>

#include <TH2F.h>
#include <TProfile.h>
#include <TSystem.h>
#include <TStyle.h>

class HitPairsTimeDistance : Bisa::Plot
{
public:

    HitPairsTimeDistance(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 1, 1, config)
     , color_(Form("%s_color", name_))
     , profile_(Form("%s_profile", name_))
    {
        init();
    }

    ~HitPairsTimeDistance()
    {
        configure();
        save();
    }

    void addHits(Bisa::HitCollection hits)
    {
        for (auto &&hit : hits)
        {
            for (auto &&other : hits)
            {
                if (hit != other)
                {
                    if (hit.tdc() == other.tdc())
                    {
                        unsigned int distance = std::abs(strip(hit) - strip(other));
                        double delta_time = std::abs(time(hit) - time(other));

                        if (delta_time < 25)
                        {
                            color_[hit.tdc()].Fill(distance, delta_time);
                            profile_[hit.tdc()].Fill(distance, delta_time);
                        }
                    }
                }
            }
        }
    }

    void init()
    {   
        color_.init(32, 0, 32, 128, 0, 25);
        profile_.init(32, 0, 32, 0, 25);
    }

    void same_configure(TH2F& hist)
    {
        for (int strip = 0; strip < 32; strip++)
        {
            hist.GetXaxis()->SetBinLabel(strip+1, Form("%d", strip));
        }
        hist.GetXaxis()->SetLabelSize(0.04);
    }

    void configure()
    {
        color_.configure();
        profile_.configure();

        // Configure color_ titles
        color_.for_each([&] (TH2F& hist) {
            same_configure(hist);

            hist.GetXaxis()->SetTitle("Distance [strips]");
            hist.GetYaxis()->SetTitle("Time [ns]");
        });

        // Configure profile_ titles
        profile_.for_each([&] (TProfile& hist) {
            hist.GetXaxis()->SetTitle("Distance [strips]");
            hist.GetYaxis()->SetTitle("Time [ns]");
        });

        for (auto tdc = 0; tdc < 9; tdc++)
        {
            if (config_.coordinate(tdc) == Bisa::ETA)
            {
                color_[tdc].SetOption("COLZ");
                profile_[tdc].SetOption("COLZ");
            }
            else if (config_.orientation(tdc) == 0)
            {
                color_[tdc].SetOption("COL");
                profile_[tdc].SetOption("COL");
            }
            else
            {
                color_[tdc].SetOption("COLZ");
                profile_[tdc].SetOption("COLZ");
            }
        }
    }

    void save()
    {
        gStyle->SetOptStat(0);
        gStyle->SetOptFit(0);

        gSystem->mkdir("output/strip_mapping", true);

        auto props = Bisa::SummaryTdc<TH2F>::DrawProps();
        props.logz = true;
        color_.draw(canvas_, props);
        canvas_->Print(Form("output/strip_mapping/%s_color.pdf", name_));
        canvas_->Clear();

        profile_.draw(canvas_, Bisa::SummaryTdc<TProfile>::DrawProps());
        canvas_->Print(Form("output/strip_mapping/%s_profile.pdf", name_));
        canvas_->Clear();
    }

private:
    int strip(const Bisa::Hit& hit)
    {
        return config_.strip(hit.channel());
    }

    double time(const Bisa::Hit& hit)
    {
        return hit.bcid_tdc() * config_.bcid_resolution() + hit.fine_time() * config_.fine_time_resolution();
    }

private:
    Bisa::SummaryTdc<TH2F> color_;
    Bisa::SummaryTdc<TProfile> profile_;

};
