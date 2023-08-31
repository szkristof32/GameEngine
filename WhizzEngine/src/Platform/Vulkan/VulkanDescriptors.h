#pragma once

#include "WhizzEngine/Rendering/Descriptors.h"
#include "WhizzEngine/Rendering/Buffers.h"

#include <vulkan/vulkan.h>

namespace WhizzEngine {

	class VulkanDescriptorSetLayout : public DescriptorSetLayout
	{
	public:
		VulkanDescriptorSetLayout() = default;
		virtual ~VulkanDescriptorSetLayout();

		virtual void AddBinding(DescriptorType type, ShaderStage stage) override;

		VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_DescriptorSetLayout; }
		operator VkDescriptorSetLayout() const { return m_DescriptorSetLayout; }
	private:
		void RecreateLayout();
	private:
		VkDescriptorSetLayout m_DescriptorSetLayout = nullptr;
		std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings;
	};

	class VulkanDescriptorPool : public DescriptorPool
	{
	public:
		VulkanDescriptorPool() = default;
		virtual ~VulkanDescriptorPool();

		virtual void AddPoolType(DescriptorType type, uint32_t count) override;

		virtual std::shared_ptr<DescriptorSet> AllocateDescriptor(std::shared_ptr<DescriptorSetLayout> layout) const override;
		virtual void FreeDescriptors(std::vector<DescriptorSet>& descriptors) const override;
		virtual void Reset() override;
	private:
		void RecreatePool();
	private:
		VkDescriptorPool m_DescriptorPool;
		std::vector<VkDescriptorPoolSize> m_PoolSizes;
	};

	class VulkanDescriptorSet : public DescriptorSet
	{
	public:
		VulkanDescriptorSet(VkDescriptorSet descriptorSet);
		virtual ~VulkanDescriptorSet() = default;

		virtual void Bind(std::shared_ptr<Pipeline> pipeline, std::vector<uint32_t> dynamicOffsets = {}) override;
	private:
		VkDescriptorSet m_DescriptorSet;
	};

}