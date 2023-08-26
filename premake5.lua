include "Dependencies.lua"

workspace "WhizzEngine"
	architecture "x86_64"
	startproject "Tester"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

	prebuildcommands
	{
		("@rm -rf %{wks.location}x64")
	}

	postbuildcommands
	{
		("@rm -rf %{wks.location}x64")
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "ExternalDependencies.lua"
	include "WhizzEngine/vendor/glad"
group ""

include "WhizzEngine"
include "Tester"