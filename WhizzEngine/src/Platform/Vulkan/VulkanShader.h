#pragma once

#include "WhizzEngine/Rendering/Shader.h"

#include <vulkan/vulkan.h>

namespace WhizzEngine {

	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const std::string& filepath);
		virtual ~VulkanShader();

		virtual void Start() override;
		virtual void Stop() override;

		const std::unordered_map<VkShaderStageFlagBits, VkShaderModule>& GetShaderModules() const { return m_ShaderStages; }
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<VkShaderStageFlagBits, std::string> PreProcess(const std::string& source);

		void CompileOrGetVulkanBinaries(const std::unordered_map<VkShaderStageFlagBits, std::string>& shaderSources);
		void CreateProgram();
		void Reflect(VkShaderStageFlagBits stage, const std::vector<uint32_t>& shaderData);
	private:
		std::unordered_map<VkShaderStageFlagBits, VkShaderModule> m_ShaderStages;
		std::string m_FilePath;
		std::string m_Name;

		std::unordered_map<VkShaderStageFlagBits, std::vector<uint32_t>> m_VulkanSPIRV;
		std::unordered_map<VkShaderStageFlagBits, std::string> m_SourceCode;
	};

}