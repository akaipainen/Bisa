#pragma once
#include "bapch.h"

#include "Plot.h"

#include <TString.h>
#include <TCanvas.h>
#include <TH1.h>

namespace Bisa {

    template <typename H>
    class SummaryTdc
    {
    public:
        SummaryTdc(const char* name, bool separate=false)
         : name_(name)
         , separate_(separate)
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
         : separate_(other.separate_)
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
            std::swap(separate_, temp.separate_);
            return *this;
        }

        template <typename ... Args>
        void init(Args ... args)
        {
            for (int i = 0; i < 9; i++)
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
            return *tdcs_[i];
        }

        const H& operator[](unsigned int i) const
        {
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
            }
        }

        void draw(TCanvas* canvas, bool divideCanvas=true, const char* options="")
        {
            // Create sub-pads
            // Divide should only be false if drawing on SAME canvas
            if (divideCanvas) canvas->Divide(3, 3, 0, 0);

            for (unsigned int tdc = 0; tdc < 9; tdc++)
            {
                // Set correct sub-pad
                if (tdc < 3) canvas->cd(tdc*3+1);
                else if (tdc < 5) canvas->cd(tdc-1);
                else if (tdc < 7) canvas->cd(tdc);
                else if (tdc < 9) canvas->cd(tdc+1);

                if (!separate_)
                {
                    // Configure margins                    L    R    T    B
                    if (tdc < 3)           gPad->SetMargin(0.1, 0.1, 0.1, 0.1);
                    else if (tdc % 2 == 0) gPad->SetMargin(0.0, 0.1, 0.1, 0.1);
                    else                   gPad->SetMargin(0.1, 0.0, 0.1, 0.1);
                }
                else
                {
                    // Configure margins to be equal
                    gPad->SetMargin(0.1, 0.1, 0.1, 0.1);
                }

                gPad->SetLogy();
                
                tdcs_[tdc]->Draw(options);
            }
        }

    private:
        std::vector<H*> tdcs_;
        const char* name_;
        bool separate_;
    };

}