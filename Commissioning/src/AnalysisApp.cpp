#include <Bisa.h>
#include <Bisa/EntryPoint.h>

#include "HitSelector.h"

#include "basic/basic.h"
#include "efficiency/efficiency.h"
#include "pad_trigger/pad_trigger.h"
#include "scintillator_trigger/scintillator_trigger.h"

class AnalysisApp : public Bisa::Application
{
public:
    AnalysisApp(const Bisa::Config &config)
     : Bisa::Application(config)
     , hitSelector_(config)
     , basic_("basic", tree_.get(), config)
     , efficiency_("efficiency", config.voltage(), tree_.get(), config)
     , pad_trigger_("pad_trigger", tree_.get(), config)
     , scint_trigger_("scint_trigger", tree_.get(), config)
    {
    }

    ~AnalysisApp()
    {
    }

    void Run() override
    {
        while (dataStream_->fill_next_event())
        {
            Step();
        }
        file_->Write();
    }

    void Step() override
    {
        Bisa::HitCollection first_hits = hitSelector_.firstHits(hits_);

        basic_.add_hits(hits_);
        efficiency_.add_hits(hits_);
        pad_trigger_.add_hits(hits_);
        scint_trigger_.add_hits(hits_);

        // tree_->Fill(); // fill the tree with the data from this step
    }

private:
    // Hit/Feature selectors
    HitSelector hitSelector_;

    // Experiments
    Basic basic_;
    Efficiency efficiency_;
    PadTrigger pad_trigger_;
    ScintillatorTrigger scint_trigger_;
};

Bisa::Application* Bisa::CreateApplication(const Bisa::Config& config)
{
    return new AnalysisApp(config);
}