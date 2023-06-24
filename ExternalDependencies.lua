externalproject "spdlog"
	location "WhizzEngine/vendor/spdlog"
	uuid "57940020-8E99-AEB6-271F-61E0E7F6B73B"
	kind "StaticLib"
	language "C"
	staticruntime "off"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

externalproject "glm_static"
	location "WhizzEngine/vendor/glm/glm"
	uuid "57940020-8E99-AEB6-271F-61E0D7F6B73B"
	kind "StaticLib"
	language "C"
	staticruntime "off"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

--[[
externalproject "vk-bootstrap"
	location "WhizzEngine/vendor/vk-bootstrap"
	uuid "57940020-8E99-AEB6-271F-61E0C7F6B73B"
	kind "StaticLib"
	language "C++"
	staticruntime "off"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
externalproject "VulkanMemoryAllocator"
	location "WhizzEngine/vendor/VMA/src"
	uuid "57940020-8E99-AEB6-271F-61E0B7F6B73B"
	kind "StaticLib"
	language "C++"
	staticruntime "off"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
]]--