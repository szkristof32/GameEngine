VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["Glad"] = "%{wks.location}/WhizzEngine/vendor/Glad/include"
IncludeDir["spdlog"] = "%{wks.location}/WhizzEngine/vendor/spdlog/include"
IncludeDir["glm"] = "%{wks.location}/WhizzEngine/vendor/glm"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["stb_image"] = "%{wks.location}/WhizzEngine/vendor/stb_image"
IncludeDir["ImGui"] = "%{wks.location}/WhizzEngine/vendor/ImGui"
IncludeDir["nfd"] = "%{wks.location}/WhizzEngine/vendor/nativefiledialog/src/include"
IncludeDir["vkb"] = "%{wks.location}/WhizzEngine/vendor/vk-bootstrap/src"
IncludeDir["VMA"] = "%{wks.location}/WhizzEngine/vendor/VMA/include"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"