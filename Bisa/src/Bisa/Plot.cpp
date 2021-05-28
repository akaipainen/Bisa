#include "Plot.h"

#include <TDirectory.h>
#include <TString.h>
#include <TSystem.h>

namespace Bisa
{
    Plot::Plot(const char *name, const char *title, Experiment *experiment)
    : name_(name)
    , canvas_(make_unique<TCanvas>(name, title, 500, 300))
    , experiment_(experiment)
    {
    }

    Plot::~Plot()
    {
    }

    TCanvas *const Plot::canvas() const
    {
        return canvas_.get();
    }

    void Plot::print(const char *name, const char *folder) const
    {
        if (strcmp(folder, "") == 0)
        {
            if (strcmp(name, "") == 0)
            {
                canvas_->Print(Form("%s/%s.pdf", experiment_->path().Data(), name_));
            } else {
                canvas_->Print(Form("%s/%s.pdf", experiment_->path().Data(), name));
            }
        } else {
            gSystem->mkdir(Form("%s/%s", experiment_->path().Data(), folder), true);
            if (strcmp(name, "") == 0)
            {
                canvas_->Print(Form("%s/%s/%s.pdf", experiment_->path().Data(), folder, name_));
            } else {
                canvas_->Print(Form("%s/%s/%s.pdf", experiment_->path().Data(), folder, name));
            }
        }
    }
}