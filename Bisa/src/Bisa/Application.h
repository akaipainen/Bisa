#pragma once

#include "root/TFile.h"
#include "root/TTree.h"

#include "Bisa/Core.h"
#include "Bisa/Config.h"
#include "Bisa/Features/HitCollection.h"
#include "Bisa/Data/DataStream.h"

int main(int argc, char** argv);

namespace Bisa {
    
    class BISA_API Application
    {
    public:
        Application(const Config& config);
        virtual ~Application();

        virtual void Run();

        virtual void Step();

    protected:
        HitCollection hits_;
        Scope<DataStream> dataStream_;
        Scope<TFile> file_;
        Scope<TTree> tree_;

        Config config_;

    private:
        friend int ::main(int argc, char** argv);

    };

    // To be defined in CLIENT
    Application* CreateApplication(const Config& config);
    
}