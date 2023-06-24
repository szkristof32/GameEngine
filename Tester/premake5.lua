project "Tester"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"src/**.cpp",
		"src/**.h"
	}
	
	includedirs
	{
		"src",
		"%{wks.location}/WhizzEngine/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}"
	}
	
	links
	{
		"WhizzEngine"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX"
	}
	
	filter "system:windows"
		systemversion "latest"
	
	filter "configurations:Debug"
		defines "WZ_DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "WZ_RELEASE"
		runtime "Release"
		optimize "on"
		
	filter "configurations:Dist"
		defines "WZ_DIST"
		runtime "Release"
		optimize "on"