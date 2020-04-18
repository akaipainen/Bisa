workspace "Bisa"
    architecture "x86_64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Bisa"
    location "Bisa"
    kind "SharedLib"
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
        -- "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include"
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
        "Bisa/vendor/spdlog/include",
        "Bisa/src",
        "Bisa/vendor"
    }

    links
    {
        "Bisa"
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