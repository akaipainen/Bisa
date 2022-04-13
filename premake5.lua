workspace "Bisa"
    architecture "x86_64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    newoption
    {
        trigger = "rootsys",
        value = "path",
        description = "Path to root directory of ROOT installation"
    }

    if not _OPTIONS["rootsys"] then
        _OPTIONS["rootsys"] = "/usr/local"
    end

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

function includeRoot()
    includedirs 
    {
        _OPTIONS["rootsys"] .. "/include/root",
        _OPTIONS["rootsys"] .. "/root/include"
    }
end

function linkRoot()
    libdirs 
    {
        _OPTIONS["rootsys"] .. "/lib/root",
        _OPTIONS["rootsys"] .. "/root/lib"
    }

    filter "system:linux"
        libdirs
        {
            _OPTIONS["rootsys"] .. "/lib64/root",
            _OPTIONS["rootsys"] .. "/root/lib64"
        }

    filter "kind:not StaticLib"
        links 
        {
            "Core", "Imt", "RIO", "Net", "Hist", 
            "Graf", "Graf3d", "Gpad", "ROOTVecOps", 
            "Tree", "TreePlayer", "Rint", "Postscript",
            "Matrix", "Physics", "MathCore", "Thread",
            "MultiProc", "ROOTDataFrame", "pthread"
        }
    filter {}
end

project "Bisa"
    location "Bisa"
    kind "SharedLib"
    language "C++"
    cppdialect "C++11"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    includeRoot()
    linkRoot()

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/vendor/json/include",
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src"
    }

    pchheader "bapch.h"
    pchsource "Bisa/src/bapch.cpp"

    filter "configurations:"

        defines
        {
            "BA_DYNAMIC_LINK",
            "BA_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Commissioning")
        }

    filter "system:linux"
        -- buildoptions 
        -- {
        --     "-std=c++1y"
        -- }

    filter "system:macosx"
    -- cppdialect "C++11"

    filter "system:windows"
        systemversion "latest"
        -- cppdialect "C++11"

    filter "configurations:Debug"
        defines "BA_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "BA_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "BA_DIST"
        runtime "Release"
        optimize "on"

function useBisa()
    includeRoot()
    linkRoot()

    includedirs 
    {
        "Bisa/src",
        "Bisa/vendor/json/include",
        "Bisa/vendor/spdlog/include"
    }
    links "Bisa"
    
end

project "Commissioning"
    location "Commissioning"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++11"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    
    useBisa()

    filter "configurations:"

        defines
        {
            "BA_PLATFORM_MACOS"
        }
    
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "BA_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "BA_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "BA_DIST"
        runtime "Release"
        optimize "on"