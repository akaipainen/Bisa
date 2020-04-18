#include <Bisa.h>

class AnalysisApp : public Bisa::Application
{
public:
    AnalysisApp()
    {

    }

    ~AnalysisApp()
    {

    }
};

Bisa::Application* Bisa::CreateApplication()
{
    return new AnalysisApp();
}
