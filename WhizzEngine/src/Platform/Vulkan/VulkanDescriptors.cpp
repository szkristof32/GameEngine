#include "pch.h"
#include "VulkanDescriptors.h"

#include "WhizzEngine/Core/Application.h"
#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "Platform/Vulkan/VulkanPipeline.h"

namespace WhizzEngine {

	namespace Utils {

		static VkDescriptorType DescriptorTypeToVKEnum(DescriptorType type)
		{
			switch (type)
			{
				case DescriptorType::UniformBuffer:			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				case DescriptorType::DynamicUniformBuffer:	return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
				case DescriptorType::ImageSampler:			return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			}

			WZ_CORE_ASSERT(false, "Invalid DescriptorType!");
			return (VkDescriptorType)0;
		}

		static VkShaderStageFlags ShaderStageToVKEnum(ShaderStage stage)
		{
			VkShaderStageFlags stageFlags = 0;

			if (stage & ShaderStage::VertexShader)
				stageFlags |= VK_SHADER_STAGE_VERTEX_BIT;
			if (stage & ShaderStage::FragmentShader)
				stageFlags |= VK_SHADER_STAGE_FRAGMENT_BIT;
			if (stage & ShaderStage::GeomertyShader)
				stageFlags |= VK_SHADER_STAGE_GEOMETRY_BIT;

			return stageFlags;
		}

	}

	VulkanDescriptorSet::VulkanDescriptorSet(VkDescriptorSet descriptorSet)
		: m_DescriptorSet(descriptorSet)
	{
	}

	void VulkanDescriptorSet::Bind(std::shared_ptr<Pipeline> pipeline, std::vector<uint32_t> dynamicOffsets)
	{
		auto& renderer = Application::Get().GetRendererAPI()->As<VulkanRendererAPI>();
		vkCmdBindDescriptorSets(renderer.GetCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->As<VulkanPipeline>().GetPipelineLayout(), 0, 1, &m_DescriptorSet, (uint32_t)dynamicOffsets.size(), dynamicOffsets.data());
	}

	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		vkDestroyDescriptorPool(Application::Get().GetContext()->As<VulkanContext>(), m_DescriptorPool, nullptr);
	}

	void VulkanDescriptorPool::AddPoolType(DescriptorType type, uint32_t count)
	{
		m_PoolSizes.push_back({ Utils::DescriptorTypeToVKEnum(type), count });
		RecreatePool();
	}

	std::shared_ptr<DescriptorSet> VulkanDescriptorPool::AllocateDescriptor(std::shared_ptr<DescriptorSetLayout> layout) const
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.pSetLayouts = (VkDescriptorSetLayout*)&layout->As<VulkanDescriptorSetLayout>();
		allocInfo.descriptorSetCount = 1;

		VkDescriptorSet set;
		WZ_CORE_ASSERT(vkAllocateDescriptorSets(Application::Get().GetContext()->As<VulkanContext>(), &allocInfo, &set) == VK_SUCCESS, "Failed to create descriptor set!");
		return std::make_shared<VulkanDescriptorSet>(set);
	}

	void VulkanDescriptorPool::FreeDescriptors(std::vector<DescriptorSet>& descriptors) const
	{
		vkFreeDescriptorSets(Application::Get().GetContext()->As<VulkanContext>(), m_DescriptorPool, (uint32_t)descriptors.size(), (VkDescriptorSet*)descriptors.data());
	}

	void VulkanDescriptorPool::Reset()
	{
		vkResetDescriptorPool(Application::Get().GetContext()->As<VulkanContext>(), m_DescriptorPool, 0);
	}

	void VulkanDescriptorPool::RecreatePool()
	{
		vkDestroyDescriptorPool(Application::Get().GetContext()->As<VulkanContext>(), m_DescriptorPool, nullptr);

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = (uint32_t)m_PoolSizes.size();
		poolInfo.pPoolSizes = m_PoolSizes.data();
		poolInfo.maxSets = 1000;
		poolInfo.flags = 0;

		WZ_CORE_ASSERT(vkCreateDescriptorPool(Application::Get().GetContext()->As<VulkanContext>(), &poolInfo, nullptr, &m_DescriptorPool) == VK_SUCCESS, "Failed to create descriptor pool!");
	}

	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(Application::Get().GetContext()->As<VulkanContext>(), m_DescriptorSetLayout, nullptr);
	}

	void VulkanDescriptorSetLayout::AddBinding(DescriptorType type, ShaderStage stage)
	{
		VkDescriptorSetLayoutBinding layoutBinding{};
		layoutBinding.binding = (uint32_t)m_Bindings.size();
		layoutBinding.descriptorType = Utils::DescriptorTypeToVKEnum(type);
		layoutBinding.descriptorCount = 1;
		layoutBinding.stageFlags = Utils::ShaderStageToVKEnum(stage);
		m_Bindings[(uint32_t)m_Bindings.size()] = layoutBinding;

		RecreateLayout();
	}

	void VulkanDescriptorSetLayout::RecreateLayout()
	{
		if (m_DescriptorSetLayout)
			vkDestroyDescriptorSetLayout(Application::Get().GetContext()->As<VulkanContext>(), m_DescriptorSetLayout, nullptr);

		std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
		for (auto&& [index, binding] : m_Bindings)
		{
			setLayoutBindings.push_back(binding);
		}

		VkDescriptorSetLayoutCreateInfo setLayoutInfo{};
		setLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		setLayoutInfo.bindingCount = (uint32_t)setLayoutBindings.size();
		setLayoutInfo.pBindings = setLayoutBindings.data();

		WZ_CORE_ASSERT(vkCreateDescriptorSetLayout(Application::Get().GetContext()->As<VulkanContext>(), &setLayoutInfo, nullptr, &m_DescriptorSetLayout) == VK_SUCCESS, "Failed to create descriptor set layout!");
	}

}