#pragma once
#include "bapch.h"

#include <TString.h>
#include <TCanvas.h>

namespace Bisa {

    template <typename H>
    class SummaryTdc
    {
    public:
        SummaryTdc(std::string name, bool separate=false)
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
                        Form("%s_tdc_%d", name_.c_str(), i),
                        Form("TDC %d", i),
                        std::forward<Args>(args)...
                    )
                );
            }
        }

        // TODO: Implement fill command with variable number of arguments and
        // with both Hit and Feature Collections (first merge those)
        // void fill()

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
                f(tdc);
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

        void draw(TCanvas* canvas, bool divide=true, const char* options="")
        {
            // Create sub-pads
            // Divide should only be false if drawing on SAME canvas
            if (divide) canvas->Divide(3, 3, 0, 0);

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
                
                tdcs_[tdc]->Draw(options);
            }
        }

    private:
        std::vector<H*> tdcs_;
        std::string name_;
        bool separate_;
    };

}