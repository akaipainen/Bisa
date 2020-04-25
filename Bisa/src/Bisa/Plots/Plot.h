#pragma once
#include "bapch.h"

#include <TCanvas.h>

namespace Bisa {

    class Plot
    {
    public:
        Plot(const char* name, int width, int height)
        : name_(name)
        , canvas_(new TCanvas(name, "", width*500, height*300))
        {   
        }

        virtual ~Plot()
        {
            delete canvas_;
        }

    protected:
        const char* name_;
        TCanvas* canvas_;

        static const bool tdc_directions_[9];
        static const int strip_mapping_[32];
    };

}
