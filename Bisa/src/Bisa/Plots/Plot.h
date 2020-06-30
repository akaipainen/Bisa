#pragma once
#include "bapch.h"

#include <TCanvas.h>

namespace Bisa {

    class Plot
    {
    public:
        Plot(const char* name, int width, int height, const Config& config)
        : name_(name)
        , canvas_(new TCanvas(name, "", width*500, height*300))
        , config_(config)
        {   
            if (!gDirectory->GetDirectory(name_))
            {
                gDirectory->mkdir(name_);
            }
        }

        virtual ~Plot()
        {
            delete canvas_;
        }

    protected:
        const char* name_;
        TCanvas* canvas_;
        
        const Config& config_;
    };

}
