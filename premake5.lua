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

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "ExternalDependencies.lua"
	include "WhizzEngine/vendor/glad"
group ""

include "WhizzEngine"
include "Tester"