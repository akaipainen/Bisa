#include "Experiment.h"

#include <TSystem.h>

namespace Bisa
{
    Experiment::Experiment(const char *name, TTree *tree, const Config &config)
     : name_(name)
     , tree_(tree)
     , config_(config)
    {
        gSystem->mkdir(Form("%s/%s", config_.output_path().c_str(), name), true);
        gDirectory->cd("/");
        gDirectory->mkdir(Form("%s", name));
        gDirectory->cd(Form("%s", name));
    }

    Experiment::~Experiment()
    {
    }

    TString Experiment::name()
    {
        return TString(name_);
    }

    TTree * Experiment::tree()
    {
        return tree_;
    }

}