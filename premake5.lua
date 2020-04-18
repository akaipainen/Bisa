workspace "Bisa"
    architecture "x86_64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

ROOTLibs = {
    "Core",
    "Hist",
    "RIO",
    "Tree",
    "Gpad"
}

project "Bisa"
    location "Bisa"
    kind "StaticLib"
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

    includedirs
    {
        "%{prj.name}/vendor/json/include",
        "%{prj.name}/vendor/spdlog/include",
        "/usr/local/Cellar/root/6.18.04_1/include/root"
    }

    libdirs
    {
        "/usr/local/lib/root"
    }
    links
    {
        ROOTLibs
    }

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

        defines
        {
            "BA_PLATFORM_MACOS"
        }

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

    includedirs
    {
        "Bisa/vendor/json/include",
        "Bisa/vendor/spdlog/include",
        "Bisa/src",
        "/usr/local/Cellar/root/6.18.04_1/include/root"
    }
    
    libdirs
    {
        "/usr/local/lib/root"
    }
    links
    {
        "Bisa",
        ROOTLibs
    }

    filter "configurations:"

        defines
        {
            "BA_PLATFORM_MACOS"
        }

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