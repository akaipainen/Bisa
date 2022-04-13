#pragma once

#include <TFile.h>
#include <TTree.h>

#include "Bisa/Core.h"
#include "Bisa/Config.h"
#include "Bisa/Features/HitCollection.h"
#include "Bisa/Data/DataStream.h"

int main(int argc, char** argv);

namespace Bisa {
    class BISA_API Application
    {
    public:
        Application(const char *datafile, const char *output_path);
        virtual ~Application();

        virtual void Run();

        virtual void Step();

    protected:
        HitCollection hits_;
        DataStream data_stream_;
        TFile *file_;
        TTree *tree_;

        std::string output_path_;

    private:
        friend int ::main(int argc, char** argv);

    };

    // To be defined in CLIENT
    Application* CreateApplication(const char *datafile, const char *output_path);
    
}