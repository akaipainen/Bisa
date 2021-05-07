#include "Plot.h"

#include <TDirectory.h>
#include <TString.h>
#include <TSystem.h>

namespace Bisa
{
    Plot::Plot(const char *name, const char *title, Experiment *experiment, const Config& config)
    : name_(name)
    , canvas_(make_unique<TCanvas>(name, title, 500, 300))
    , experiment_(experiment)
    , config_(config)
    {
    }

    Plot::~Plot()
    {
    }

    TCanvas *const Plot::canvas() const
    {
        return canvas_.get();
    }

    // void Plot::set_experiment(Experiment *experiment)
    // {
    //     experiment_ = experiment;
    // }

    void Plot::print(const char *name, const char *folder) const
    {
        if (strcmp(folder, "") == 0)
        {
            if (strcmp(name, "") == 0)
            {
                canvas_->Print(Form("%s/%s/%s.pdf", config_.output_path().c_str(), experiment_->name().Data(), name_));
            } else {
                canvas_->Print(Form("%s/%s/%s.pdf", config_.output_path().c_str(), experiment_->name().Data(), name));
            }
        } else {
            gSystem->mkdir(Form("%s/%s/%s", config_.output_path().c_str(), experiment_->name().Data(), folder), true);
            if (strcmp(name, "") == 0)
            {
                canvas_->Print(Form("%s/%s/%s/%s.pdf", config_.output_path().c_str(), experiment_->name().Data(), folder, name_));
            } else {
                canvas_->Print(Form("%s/%s/%s/%s.pdf", config_.output_path().c_str(), experiment_->name().Data(), folder, name));
            }
        }
    }
}