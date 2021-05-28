#include "Experiment.h"

#include <TSystem.h>

namespace Bisa
{
    Experiment::Experiment(const char *name, TTree *tree, const char *output_path)
     : name_(name)
     , tree_(tree)
     , path_(Form("%s/%s", output_path, name))
    {
        gSystem->mkdir(path_.c_str(), true);
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

    TString Experiment::path()
    {
        return path_;
    }

    TTree * Experiment::tree()
    {
        return tree_;
    }

}