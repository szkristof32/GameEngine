#include "pch.h"
#include "VulkanShader.h"

#include "WhizzEngine/Core/Engine.h"
#include "Platform/Vulkan/VulkanContext.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>
#include <fstream>

namespace WhizzEngine {

	namespace Utils {

		static VkShaderStageFlagBits ShaderTypeFromString(const std::string& type)
		{
			if (type == "vertex")
				return VK_SHADER_STAGE_VERTEX_BIT;
			if (type == "fragment" || type == "pixel")
				return VK_SHADER_STAGE_FRAGMENT_BIT;

			WZ_CORE_ASSERT(false, "Unknown shader type!");
			return (VkShaderStageFlagBits)0;
		}

		static shaderc_shader_kind VKShaderStageToShaderC(VkShaderStageFlagBits stage)
		{
			switch (stage)
			{
				case VK_SHADER_STAGE_VERTEX_BIT:	return shaderc_glsl_vertex_shader;
				case VK_SHADER_STAGE_FRAGMENT_BIT:	return shaderc_glsl_fragment_shader;
			}
			
			WZ_CORE_ASSERT(false, "Unknown shader type!");
			return (shaderc_shader_kind)0;
		}

		static const char* VKShaderStageToString(VkShaderStageFlagBits stage)
		{
			switch (stage)
			{
				case VK_SHADER_STAGE_VERTEX_BIT:	return "VK_SHADER_STAGE_VERTEX_BIT";
				case VK_SHADER_STAGE_FRAGMENT_BIT:	return "VK_SHADER_STAGE_FRAGMENT_BIT";
			}

			WZ_CORE_ASSERT(false, "Unknown shader type!");
			return "";
		}

		static const char* GetCacheDirectory()
		{
			return "assets/cache/shader/vulkan";
		}

		static void CreateCacheDirectoryIfNeeded()
		{
			std::string cacheDirectory = GetCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}

		static const char* VKShaderStageCachedVulkanFileExtension(VkShaderStageFlagBits stage)
		{
			switch (stage)
			{
				case VK_SHADER_STAGE_VERTEX_BIT:	return ".cached_vulkan.vert";
				case VK_SHADER_STAGE_FRAGMENT_BIT:	return ".cached_vulkan.frag";
			}

			WZ_CORE_ASSERT(false, "Unknown shader type!");
			return "";
		}

	}

	VulkanShader::VulkanShader(const std::string& filepath)
		: m_FilePath(filepath)
	{
		Utils::CreateCacheDirectoryIfNeeded();

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);

		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);

		{
			// TODO: create timers
			CompileOrGetVulkanBinaries(shaderSources);
			CreateProgram();
		}
	}

	VulkanShader::~VulkanShader()
	{
		auto& context = Engine::GetContext()->As<VulkanContext>();

		for (auto&& [stage, shaderModule] : m_ShaderStages)
		{
			vkDestroyShaderModule(context, shaderModule, nullptr);
		}
	}

	void VulkanShader::Start()
	{
	}

	void VulkanShader::Stop()
	{
	}

	std::string VulkanShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary | std::ios::ate);
		if (in)
		{
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				WZ_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			WZ_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<VkShaderStageFlagBits, std::string> VulkanShader::PreProcess(const std::string& source)
	{
		std::unordered_map<VkShaderStageFlagBits, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			WZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			WZ_CORE_ASSERT(Utils::ShaderTypeFromString(type) != VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM, "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			WZ_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos);

			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void VulkanShader::CompileOrGetVulkanBinaries(const std::unordered_map<VkShaderStageFlagBits, std::string>& shaderSources)
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_1);
		const bool optimise = true;
		if (optimise)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : shaderSources)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::VKShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary | std::ios::ate);
			if (in.is_open())
			{
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::VKShaderStageToShaderC(stage), m_FilePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					WZ_CORE_ERROR(module.GetErrorMessage());
					WZ_CORE_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data] : shaderData)
			Reflect(stage, data);
	}

	void VulkanShader::CreateProgram()
	{
		for (auto&& [stage, spirv] : m_VulkanSPIRV)
		{
			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.codeSize = spirv.size() * sizeof(uint32_t);
			createInfo.pCode = spirv.data();

			VkShaderModule shaderModule;
			WZ_CORE_ASSERT(vkCreateShaderModule(Engine::GetContext()->As<VulkanContext>(), &createInfo, nullptr, &shaderModule) == VK_SUCCESS, "Failed to create shader module!");
			m_ShaderStages[stage] = shaderModule;
		}
	}

	void VulkanShader::Reflect(VkShaderStageFlagBits stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		WZ_CORE_TRACE("Shader::Reflect - {0} {1}", Utils::VKShaderStageToString(stage), m_FilePath);
		WZ_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
		WZ_CORE_TRACE("    {0} resources", resources.sampled_images.size());

		WZ_CORE_TRACE("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = (uint32_t)compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = (int)bufferType.member_types.size();

			WZ_CORE_TRACE("  {0}", resource.name);
			WZ_CORE_TRACE("    Size = {0}", bufferSize);
			WZ_CORE_TRACE("    Binding = {0}", binding);
			WZ_CORE_TRACE("    Members = {0}", memberCount);
		}
	}

}