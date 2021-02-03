#pragma once
#include "bapch.h"

#include "Plot.h"
#include "Bisa/Config.h"

#include <TString.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TH1.h>

namespace Bisa {

    template <typename H>
    class SummaryAgg
    {
    public:

        struct DrawProps {
            bool logy;
            bool logz;
            const char* options;
        };

        SummaryAgg(const char* name, const Config& config)
         : name_(name)
         , config_(config)
        {
        }

        ~SummaryAgg()
        {
            delete agg_;
        }

        SummaryAgg(const SummaryAgg<H>& other)
         : name_(other.name_)
         , config_(other.config_)
        {
            agg_ = new H(other->agg_);
        }

        SummaryAgg<H>& operator=(const SummaryAgg<H>& other)
        {
            auto temp(other);
            std::swap(agg_, temp.agg_);
            std::swap(config_, temp.config_);
            return *this;
        }

        template <typename ... Args>
        void init(Args ... args)
        {
            agg_ = new H(
                    Form("%s_agg", name_.c_str()),
                    "",
                    std::forward<Args>(args)...
            );
        }

        unsigned int size() const
        {
            return agg_->GetEntries();
        }

        H& operator()()
        {
            return *agg_;
        }

        void apply(std::function<void(H&)> f)
        {
            f(*agg_);
        }

        void setTitles(const std::string& xtitle, const std::string& ytitle)
        {
            agg_->GetXaxis()->SetTitle(xtitle);
            agg_->GetYaxis()->SetTitle(ytitle);
        }

        void configure()
        {
            // Set histogram maximum
            double max = agg_->GetMaximum();
            agg_->SetMaximum(max * 1.1);
            agg_->SetMinimum(0);
        }

        void draw(TCanvas* canvas, DrawProps props=DrawProps())
        {
            if (props.logy) 
            {
                agg_->SetMinimum(1);
                gPad->SetLogy();
            }
            if (props.logz) 
            {
                agg_->SetMinimum(1);
                gPad->SetLogz();
            }
            if (!props.logy && !props.logz)
            {
                agg_->SetMinimum(0);
                gPad->SetLogy(false);
                gPad->SetLogz(false);
            }
            
            agg_->Draw(props.options);
        }

        void print(const char* path, DrawProps props = DrawProps())
        {
            TCanvas canvas("canvas", "print canvas", 500, 300);

            if (props.logy)
            {
                gSystem->mkdir(Form("%s/log", path), true);
                gSystem->mkdir(Form("%s/lin", path), true);

                props.logy = false;
                draw(&canvas, props);
                canvas.Print(Form("%s/lin/%s_lin.pdf", path, name_.c_str()));
                canvas.Clear();

                props.logy = true;
                draw(&canvas, props);
                canvas.Print(Form("%s/log/%s_log.pdf", path, name_.c_str()));
                canvas.Clear();
            }

            if (props.logz)
            {
                gSystem->mkdir(Form("%s/log", path), true);
                gSystem->mkdir(Form("%s/lin", path), true);

                props.logz = false;
                draw(&canvas, props);
                canvas.Print(Form("%s/lin/%s_lin.pdf", path, name_.c_str()));
                canvas.Clear();

                props.logz = true;
                draw(&canvas, props);
                canvas.Print(Form("%s/log/%s_log.pdf", path, name_.c_str()));
                canvas.Clear();
            }

            if (!props.logy && !props.logz)
            {
                gSystem->mkdir(Form("%s/lin", path), true);

                draw(&canvas, props);
                canvas.Print(Form("%s/lin/%s_lin.pdf", path, name_.c_str()));
                canvas.Clear();
            }
        }

    private:
        const Config& config_;
        
        H* agg_;
        std::string name_;

    };

}