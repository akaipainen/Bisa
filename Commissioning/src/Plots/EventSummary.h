#pragma once

#include <Bisa.h>

#include <TH2F.h>
#include <TSystem.h>
#include <TStyle.h>

class EventSummary : Bisa::Plot
{
public:
    EventSummary(const char* name, const Bisa::Config& config)
     : Bisa::Plot(name, 1, 1, config)
    {
    }

    ~EventSummary()
    {
        save();
    }

    void configureAllHits(Bisa::HitCollection hits)
    {
        std::vector<double> times;
        for (auto &&h : hits)
        {
            times.push_back(time(h));
        }
        std::sort(times.begin(), times.end());
        
        first_time_ = times[times.size() / 2] - 15*config_.fine_time_resolution();
        // BA_INFO("Event ID: {} | Start time: {}", hits.trigger_id(), first_time_);

        event_id_ = hits.trigger_id();
        // first_time_ = time(*itHit);
        // for (itHit++; itHit != hits.end(); itHit++)
        // {
        //     auto t = time(*itHit);
        //     if (t < first_time_)
        //     {
        //         first_time_ = t;
        //     }
        //     if (t > first_time_ + 1) // If more then one away
        // }
    }

    void addHits(Bisa::HitCollection hits, EColor color)
    {
        gDirectory->cd("event_display");
        
        Bisa::SummaryTdc<TH2F> layer(Form("%s_%lu", name_, layers_.size()), config_);
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
            layer[hit.tdc()].Fill(strip(hit), time(hit) - first_time_ + 5);
        }

        layers_.push_back(layer);

        gDirectory->cd("..");
    }

    void save()
    {
        gStyle->SetOptStat(0);
        gStyle->SetOptFit(0);

        Bisa::SummaryTdc<TH2F>::DrawProps props = Bisa::SummaryTdc<TH2F>::DrawProps();
        props.options = "BOX";
        props.bis7 = true;
        props.divide = true; 
        
        Bisa::SummaryTdc<TH2F>::DrawProps sameprops = Bisa::SummaryTdc<TH2F>::DrawProps();
        sameprops.options = "SAME BOX";
        sameprops.bis7 = true;
        sameprops.divide = false; 

        // Print first canvas with something on it
        for (auto &&l : layers_)
        {
            if (l.size() > 0) 
            {
                l.draw(canvas_, props);
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
            itLayer->draw(canvas_, sameprops);
        }

        gSystem->mkdir(Form("output/event_display/%s", name_), true);
        canvas_->Print(Form("output/event_display/%s/event_%u.pdf", name_, event_id_));
    }

private:
    // TODO: Move time into config file
    double time(const Bisa::Hit& hit) const
    {
        return hit.bcid_tdc() * config_.bcid_resolution() + hit.fine_time() * config_.fine_time_resolution();
    }

    unsigned int strip(const Bisa::Hit& hit)
    {
        return config_.strip(hit.channel());
    }

private:
    std::list<Bisa::SummaryTdc<TH2F>> layers_;
    
    double first_time_;
    unsigned int event_id_;

    static bool created_dir_;
};
