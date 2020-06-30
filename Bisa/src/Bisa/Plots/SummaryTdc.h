#pragma once
#include "bapch.h"

#include "Plot.h"
#include "Bisa/Config.h"

#include <TString.h>
#include <TCanvas.h>
#include <TH1.h>

namespace Bisa {

    template <typename H>
    class SummaryTdc
    {
    public:

        struct DrawProps {
            bool bis7;
            bool logy;
            bool logz;
            const char* options;
        };

        SummaryTdc(const char* name, const Config& config)
         : name_(name)
         , config_(config)
        {
        }

        ~SummaryTdc()
        {
            for (auto &&tdc : tdcs_)
            {
                delete tdc;
            }
        }

        SummaryTdc(const SummaryTdc<H>& other)
         : name_(other.name_)
         , config_(other.config_)
        {
            for (auto &&tdc : other.tdcs_)
            {
                tdcs_.emplace_back(new H(*tdc));
            }
        }

        SummaryTdc<H>& operator=(const SummaryTdc<H>& other)
        {
            auto temp(other);
            std::swap(tdcs_, temp.tdcs_);
            std::swap(config_, temp.config_);
            return *this;
        }

        template <typename ... Args>
        void init(Args ... args)
        {
            for (int i = 0; i < 18; i++)
            {
                tdcs_.emplace_back(
                    new H(
                        Form("%s_tdc_%d", name_, i),
                        Form("TDC %d", i),
                        std::forward<Args>(args)...
                    )
                );
            }
        }

        unsigned int size() const
        {
            unsigned int sum;
            for (auto &&tdc : tdcs_)
            {
                sum += tdc->GetEntries();
            }
            return sum;
        }

        H& operator[](unsigned int i)
        {
            BA_CORE_ASSERT(i < tdcs_.size(), "Accessing TDC out of bounds.");
            return *tdcs_[i];
        }

        const H& operator[](unsigned int i) const
        {
            BA_CORE_ASSERT(i < tdcs_.size(), "Accessing TDC out of bounds.");
            return *tdcs_[i];
        }

        void for_each(std::function<void(H&)> f)
        {
            for (auto &&tdc : tdcs_)
            {
                f(*tdc);
            }
        }

        void setTitles(const std::string& xtitle, const std::string& ytitle)
        {
            for (auto &&tdc : tdcs_)
            {
                tdc.GetXaxis()->SetTitle(xtitle);
                tdc.GetYaxis()->SetTitle(ytitle);
            }
        }

        void configure()
        {
            // Set histogram maximum
            double max = 0;
            for (auto it = tdcs_.begin(); it != tdcs_.end(); it++)
            {   
                if ((*it)->GetMaximum() > max) max = (*it)->GetMaximum();
            }
            for (auto it = tdcs_.begin(); it != tdcs_.end(); it++)
            {
                (*it)->SetMaximum(max*1.1);
                (*it)->SetMinimum(0);
            }
        }

        void draw(TCanvas* canvas, DrawProps props=DrawProps())
        {
            canvas->Divide(3, 3, 0, 0);
            for (unsigned int tdc = 0; tdc < 18; tdc++)
            {
                if (config_.chamber(tdc) == (props.bis7 ? 7 : 8))
                {
                    // Set correct sub-pad
                    if (config_.coordinate(tdc) == ETA && config_.layer(tdc) == 0) { canvas->cd(1); }
                    if (config_.coordinate(tdc) == ETA && config_.layer(tdc) == 1) { canvas->cd(4); }
                    if (config_.coordinate(tdc) == ETA && config_.layer(tdc) == 2) { canvas->cd(7); }
                    if (config_.coordinate(tdc) == PHI && 
                        config_.layer(tdc) == 0 && 
                        config_.orientation(tdc) == 0) { canvas->cd(2); }
                    if (config_.coordinate(tdc) == PHI && 
                        config_.layer(tdc) == 0 && 
                        config_.orientation(tdc) == 1) { canvas->cd(3); }
                    if (config_.coordinate(tdc) == PHI && 
                        config_.layer(tdc) == 1 && 
                        config_.orientation(tdc) == 0) { canvas->cd(5); }
                    if (config_.coordinate(tdc) == PHI && 
                        config_.layer(tdc) == 1 && 
                        config_.orientation(tdc) == 1) { canvas->cd(6); }
                    if (config_.coordinate(tdc) == PHI && 
                        config_.layer(tdc) == 2 && 
                        config_.orientation(tdc) == 0) { canvas->cd(8); }
                    if (config_.coordinate(tdc) == PHI && 
                        config_.layer(tdc) == 2 && 
                        config_.orientation(tdc) == 1) { canvas->cd(9); }

                    // Configure margins                                     L    R    T    B
                    if (config_.coordinate(tdc) == ETA)     gPad->SetMargin(0.1, 0.1, 0.1, 0.1);
                    else if (config_.orientation(tdc) == 1) gPad->SetMargin(0.0, 0.1, 0.1, 0.1);
                    else                                    gPad->SetMargin(0.1, 0.0, 0.1, 0.1);

                    if (props.logy) gPad->SetLogy();
                    if (props.logz) gPad->SetLogz();
                    
                    tdcs_[tdc]->Draw(props.options);
                }
            }
        }

    private:
        const Config& config_;
        
        std::vector<H*> tdcs_;
        const char* name_;

    };

}