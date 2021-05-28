#pragma once
#include "bapch.h"

#include "Config.h"
#include "Experiment.h"

#include <TCanvas.h>

namespace Bisa {

    class Plot
    {
    public:
        // Create a new plot object
        Plot(const char* name, const char *title, Experiment *experiment);

        // Destruct a plot object
        virtual ~Plot();

        // Get the canvas for this plot
        TCanvas *const canvas() const;

        // Save a pdf of this plot to the correct folder
        void print(const char *name="", const char *folder="") const;

    protected:
        const char* name_;
        std::unique_ptr<TCanvas> canvas_;

        Experiment *experiment_;
    };

}
