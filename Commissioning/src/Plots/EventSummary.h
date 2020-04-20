#pragma once

#include <Bisa.h>

#include <TH2F.h>
#include <TSystem.h>
#include <TStyle.h>

class EventSummary : Bisa::Plot
{
public:
    EventSummary(const char* name)
     : Bisa::Plot(name, 1, 1)
    {
    }

    ~EventSummary()
    {
        save();
    }

    void configureAllHits(Bisa::HitCollection hits)
    {
        auto itHit = hits.begin();
        event_id_ = itHit->second->triggerId;
        first_time_ = time(*(itHit++)->second);
        for (; itHit != hits.end(); itHit++)
        {
            auto t = time(*itHit->second);
            if (t < first_time_)
            {
                first_time_ = t;
            }
        }
    }

    void addHits(Bisa::HitCollection hits, EColor color)
    {
        Bisa::SummaryTdc<TH2F> layer((name_ + std::to_string(layers_.size())).c_str());
        layer.init(32, 0, 32, 50/25*128, 0, 50);
        layer.for_each([&](TH2F& hist) {
            hist.SetFillColor(color);
        });
        layer.for_each([](TH2F& hist) {
            for (int strip = 0; strip < 32; strip++)
            {
                hist.GetXaxis()->SetBinLabel(strip+1, Form("%d", strip));
            }
        });
        for (auto &&hit : hits)
        {
            layer[hit.second->tdc].Fill(strip(*hit.second), time(*hit.second) - first_time_ + 5);
        }

        layers_.push_back(layer);
    }

    void save()
    {
        gStyle->SetOptStat(0);
        gStyle->SetOptFit(0);

        // Print first canvas with something on it
        for (auto &&l : layers_)
        {
            if (l.size() > 0) 
            {
                l.draw(canvas_, true, "BOX");
                break;
            }
            else {
                layers_.push_back(layers_.front());
                layers_.pop_front();
            }
        }

        auto itLayer = next(layers_.begin());
        for (; itLayer != layers_.end(); itLayer++)
        {
            itLayer->draw(canvas_, false, "BOX SAME");
        }

        gSystem->mkdir(Form("output/%s", name_), true);
        canvas_->Print(Form("output/%s/event_%u.pdf", name_, event_id_));
    }

private:
    // TODO: Move time into config file
    double time(const Bisa::Hit& hit) const
    {
        return hit.bcidTdc * 25 + hit.fineTime * 25.0/128.0;
    }

    unsigned int strip(const Bisa::Hit& hit) const
    {
        const std::vector<int> strip_mapping = {
            0, 4, 8, 12, 16, 20, 24, 28,
            1, 5, 9, 13, 17, 21, 25, 29,
            2, 6,10, 14, 18, 22, 26, 30,
            3, 7,11, 15, 19, 23, 27, 31
        };

        return strip_mapping[hit.channel];
    }

private:
    std::list<Bisa::SummaryTdc<TH2F>> layers_;
    
    double first_time_;
    unsigned int event_id_;
};
