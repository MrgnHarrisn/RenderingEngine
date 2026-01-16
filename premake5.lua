workspace "BoxEngine"
    architecture "x64"
    startproject("BoxEngine")

    configurations
    {
        "Debug",
        "Release"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    --  include directories relative to BoxEngine-Core
    IncludeDir = {}
    IncludeDir["GLFW"]      = "vendor/GLFW/include"
    IncludeDir["Glad"]      = "vendor/Glad/include"
    IncludeDir["ImGui"]     = "vendor/imgui/"
    IncludeDir["glm"]       = "vendor/glm"
    IncludeDir["stb_image"] = "vendor/stb_image"
    IncludeDir["assimp"]    = "vendor/assimp"

    --projects
    group "Dependencies"
        include "BoxEngine-Core/vendor/GLFW"
        include "BoxEngine-Core/vendor/Glad"
        include "BoxEngine-Core/vendor/imgui"
        include "BoxEngine-Core/vendor/assimp"
    group ""

    include "BoxEngine-Core"
    include "BoxEngine"