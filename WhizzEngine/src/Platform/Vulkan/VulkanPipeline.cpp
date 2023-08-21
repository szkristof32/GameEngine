#include "pch.h"
#include "VulkanPipeline.h"

#include "WhizzEngine/Core/Engine.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace WhizzEngine {

	namespace Utils {

		static VkFormat DataTypeToVulkanType(DataType type)
		{
			switch (type)
			{
				case DataType::Float:	return VK_FORMAT_R32_SFLOAT;
				case DataType::Float2:	return VK_FORMAT_R32G32_SFLOAT;
				case DataType::Float3:	return VK_FORMAT_R32G32B32_SFLOAT;
				case DataType::Float4:	return VK_FORMAT_R32G32B32A32_SFLOAT;
				case DataType::Int:		return VK_FORMAT_R32_SINT;
				case DataType::Int2:	return VK_FORMAT_R32G32_SINT;
				case DataType::Int3:	return VK_FORMAT_R32G32B32_SINT;
				case DataType::Int4:	return VK_FORMAT_R32G32B32A32_SINT;
				case DataType::Mat3:	return VK_FORMAT_R32G32B32_SFLOAT;
				case DataType::Mat4:	return VK_FORMAT_R32G32B32A32_SFLOAT;
				case DataType::Bool:	return VK_FORMAT_R8_UINT;
			}
			WZ_CORE_ASSERT(false, "Invalid DataType!");
			return VK_FORMAT_MAX_ENUM;
		}

		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions(const AttributeLayout& attributeLayout)
		{
			std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
			for (auto& attribute : attributeLayout)
			{
				attributeDescriptions.push_back({ attribute.Location, 0, DataTypeToVulkanType(attribute.Type), attribute.Offset });
			}
			return attributeDescriptions;
		}

		static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions(const AttributeLayout& attributeLayout)
		{
			std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
			bindingDescriptions[0].binding = 0;
			bindingDescriptions[0].stride = attributeLayout.GetStride();
			bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			return bindingDescriptions;
		}

	}

	VulkanPipeline::VulkanPipeline(const PipelineInfo& pipelineInfo)
	{
		auto& context = Engine::GetContext()->As<VulkanContext>();

		m_Shader = Shader::Create(pipelineInfo.ShaderPath);

		std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages{};
		for (uint32_t i = 0; i < shaderStages.size(); i++)
		{
			shaderStages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStages[i].stage = i == 0 ? VK_SHADER_STAGE_VERTEX_BIT : VK_SHADER_STAGE_FRAGMENT_BIT;
			shaderStages[i].module = nullptr; // TODO
			shaderStages[i].pName = "main";
		}

		auto attributeDescriptions = Utils::GetAttributeDescriptions(pipelineInfo.AttribLayout);
		auto bindingDescriptions = Utils::GetBindingDescriptions(pipelineInfo.AttribLayout);
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)attributeDescriptions.size();
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
		vertexInputInfo.vertexBindingDescriptionCount = (uint32_t)bindingDescriptions.size();
		vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		VkPipelineViewportStateCreateInfo viewportInfo{};
		viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportInfo.viewportCount = 1;
		viewportInfo.pViewports = nullptr;
		viewportInfo.scissorCount = 1;
		viewportInfo.pScissors = nullptr;

		VkPipelineRasterizationStateCreateInfo rasterisationInfo{};
		rasterisationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterisationInfo.depthClampEnable = VK_FALSE;
		rasterisationInfo.rasterizerDiscardEnable = VK_FALSE;
		rasterisationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterisationInfo.lineWidth = 1.0f;
		rasterisationInfo.cullMode = VK_CULL_MODE_NONE;
		rasterisationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterisationInfo.depthBiasEnable = VK_FALSE;
		rasterisationInfo.depthBiasConstantFactor = 0.0f;
		rasterisationInfo.depthBiasClamp = 0.0f;
		rasterisationInfo.depthBiasSlopeFactor = 0.0f;

		VkPipelineMultisampleStateCreateInfo multisampleInfo{};
		multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleInfo.sampleShadingEnable = VK_FALSE;
		multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampleInfo.minSampleShading = 1.0f;
		multisampleInfo.pSampleMask = nullptr;
		multisampleInfo.alphaToCoverageEnable = VK_FALSE;
		multisampleInfo.alphaToOneEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState colourBlendAttachment{};
		colourBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colourBlendAttachment.blendEnable = VK_FALSE;
		colourBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		colourBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		colourBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colourBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colourBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colourBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colourBlendInfo{};
		colourBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colourBlendInfo.logicOpEnable = VK_FALSE;
		colourBlendInfo.logicOp = VK_LOGIC_OP_COPY;
		colourBlendInfo.attachmentCount = 1;
		colourBlendInfo.pAttachments = &colourBlendAttachment;
		colourBlendInfo.blendConstants[0] = 0.0f;
		colourBlendInfo.blendConstants[1] = 0.0f;
		colourBlendInfo.blendConstants[2] = 0.0f;
		colourBlendInfo.blendConstants[3] = 0.0f;

		VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
		depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilInfo.depthTestEnable = VK_TRUE;
		depthStencilInfo.depthWriteEnable = VK_TRUE;
		depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		depthStencilInfo.minDepthBounds = 0.0f;
		depthStencilInfo.maxDepthBounds = 1.0f;
		depthStencilInfo.stencilTestEnable = VK_FALSE;
		depthStencilInfo.front = {};
		depthStencilInfo.back = {};

		std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.dynamicStateCount = (uint32_t)dynamicStates.size();
		dynamicStateInfo.pDynamicStates = dynamicStates.data();

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pushConstantRangeCount = 0;

		WZ_CORE_ASSERT(vkCreatePipelineLayout(context, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) == VK_SUCCESS, "Failed to create pipeline layout!");

		VkGraphicsPipelineCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.stageCount = (uint32_t)shaderStages.size();
		createInfo.pStages = shaderStages.data();
		createInfo.pVertexInputState = &vertexInputInfo;
		createInfo.pInputAssemblyState = &inputAssemblyInfo;
		createInfo.pViewportState = &viewportInfo;
		createInfo.pRasterizationState = &rasterisationInfo;
		createInfo.pMultisampleState = &multisampleInfo;
		createInfo.pColorBlendState = &colourBlendInfo;
		createInfo.pDepthStencilState = &depthStencilInfo;
		createInfo.pDynamicState = &dynamicStateInfo;

		createInfo.layout = m_PipelineLayout;
		createInfo.renderPass = m_RenderPass;
		createInfo.subpass = 0;

		createInfo.basePipelineIndex = -1;

		WZ_CORE_ASSERT(vkCreateGraphicsPipelines(context, nullptr, 1, &createInfo, nullptr, &m_Pipeline) == VK_SUCCESS, "Failed to create graphics pipeline!");
	}

	VulkanPipeline::~VulkanPipeline()
	{
		auto& context = Engine::GetContext()->As<VulkanContext>();
		vkDestroyPipelineLayout(context, m_PipelineLayout, nullptr);
		vkDestroyRenderPass(context, m_RenderPass, nullptr);
		vkDestroyPipeline(context, m_Pipeline, nullptr);
	}

}