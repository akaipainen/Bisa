#pragma once

#include <TTree.h>

#ifdef BA_PLATFORM_MACOS

extern Bisa::Application* Bisa::CreateApplication();

int main(int argc, char** argv)
{
    Bisa::Log::Init();
    BA_CORE_WARN("Initialized Log!");
    int a = 5;
    BA_INFO("Hello! Var={0}", a);

    TTree tree("tree", "Tree");

    auto app = Bisa::CreateApplication();
    app->Run();
    delete app;
}

#endif