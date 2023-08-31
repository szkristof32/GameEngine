#pragma once

#include "WhizzEngine/Rendering/Pipeline.h"

#include <vulkan/vulkan.h>

namespace WhizzEngine {

	class VulkanPipeline : public Pipeline
	{
	public:
		VulkanPipeline(const PipelineInfo& pipelineInfo);
		~VulkanPipeline();

		virtual std::shared_ptr<Shader> GetShader() const { return m_Shader; }
		virtual VkPipelineLayout GetPipelineLayout() const { return m_PipelineLayout; }
		virtual const AttributeLayout& GetAttributeLayout() const { return m_AttributeLayout; }

		operator VkPipeline() const { return m_Pipeline; }
	private:
		VkPipeline m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
		VkRenderPass m_RenderPass;

		std::shared_ptr<Shader> m_Shader;
		AttributeLayout m_AttributeLayout;
	};

}