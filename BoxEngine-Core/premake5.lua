project "BoxEngine-Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime  "On"

    targetdir   ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir      ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files
	{
		"src/**.h",
		"src/**.cpp",
		"include/**.h",
		"include/**.cpp",
		"vendor/stb_image/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
	}

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
	{
		"src",
		"include",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.assimp}"
	}

    links {
        "GLFW",
        "Glad",
        "ImGui",
		"assimp",
        "opengl32.lib"
    }

    filter "system:windows"
		systemversion "latest"

		defines
		{
			"GLCORE_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines {"GLCORE_DEBUG"}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines {"GLCORE_RELEASE", "NDEBUG"}
		runtime "Release"
		optimize "on"