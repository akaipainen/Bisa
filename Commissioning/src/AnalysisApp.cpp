#include <Bisa.h>
#include <Bisa/EntryPoint.h>

// #include "HitSelector.h"

#include "basic/basic.h"
#include "efficiency/efficiency.h"
#include "pad_trigger/pad_trigger.h"
#include "scintillator_trigger/scintillator_trigger.h"
#include "muons/muons.h"

class AnalysisApp : public Bisa::Application
{
public:
    AnalysisApp(const char *datafile, const char *output_path)
     : Bisa::Application(datafile, output_path)
     , basic_("basic", tree_, output_path)
     , efficiency_("efficiency", 5800, tree_, output_path)
     , pad_trigger_("pad_trigger", tree_, output_path)
     , scint_trigger_("scint_trigger", tree_, output_path)
     , muons_("muons", tree_, output_path)
    {
    }

    ~AnalysisApp()
    {
    }

    void Run() override
    {
        while (data_stream_.fill_next_event())
        {
            Step();
        }
        file_->Write();
    }

    void Step() override
    {
        // Bisa::HitCollection first_hits = hitSelector_.firstHits(hits_);

        basic_.add_hits(hits_);
        efficiency_.add_hits(hits_);
        pad_trigger_.add_hits(hits_);
        scint_trigger_.add_hits(hits_);
        muons_.add_hits(hits_);

        tree_->Fill(); // fill the tree with the data from this step
    }

private:
    // Experiments
    Basic basic_;
    Efficiency efficiency_;
    PadTrigger pad_trigger_;
    ScintillatorTrigger scint_trigger_;
    Muons muons_;
};

Bisa::Application* Bisa::CreateApplication(const char *datafile, const char *output_path)
{
    return new AnalysisApp(datafile, output_path);
}