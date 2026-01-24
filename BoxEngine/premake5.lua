project "BoxEngine"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"../BoxEngine-Core/vendor/spdlog/include",
		"../BoxEngine-Core/include",
		"../BoxEngine-Core/vendor",
		"../BoxEngine-Core/%{IncludeDir.glm}",
		"../BoxEngine-Core/%{IncludeDir.Glad}",
		"../BoxEngine-Core/%{IncludeDir.GLFW}",
		"../BoxEngine-Core/%{IncludeDir.ImGui}",
		"../BoxEngine-Core/%{IncludeDir.assimp}"
	}

	links
	{
		"BoxEngine-Core"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GLCORE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "GLCORE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "GLCORE_RELEASE"
		runtime "Release"
        optimize "on"