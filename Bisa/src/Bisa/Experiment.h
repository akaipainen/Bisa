#pragma once

#include "bapch.h"
#include "Config.h"

#include "Features/HitCollection.h"

#include <TDirectory.h>
#include <TString.h>
#include <TTree.h>

namespace Bisa
{
    class Experiment
    {
    public:
        // Construct an experiment object
        Experiment(const char *name, TTree *tree, const char *output_path);

        // Destruct an experiment object and save all plots to experiment folder
        virtual ~Experiment();

        // Add event hits to this experiment
        virtual void add_hits(const Bisa::HitCollection &hits) = 0;

        // Get experiment name
        TString name();

        // Get experiment output path
        TString path();

        // Get experiment TTree
        TTree * tree();

    protected:
        // Add a plot to this experiment
        // void add_plot(Plot &&plot);

        TTree * tree_;
        const char * name_;
        std::string path_;
    };
}