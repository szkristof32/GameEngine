#pragma once

#include "WhizzEngine/Rendering/Buffers.h"

#include <vulkan/vulkan.h>

namespace WhizzEngine {

	class VulkanDescriptorSetLayout
	{
	public:
		class Builder
		{
		public:
			Builder() {}

			Builder& AddBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags, uint32_t count = 1);
			std::unique_ptr<VulkanDescriptorSetLayout> Build() const;
		private:
			std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings{};
		};

		VulkanDescriptorSetLayout(std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
		~VulkanDescriptorSetLayout();

		VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_DescriptorSetLayout; }
		operator VkDescriptorSetLayout() const { return m_DescriptorSetLayout; }
	private:
		VkDescriptorSetLayout m_DescriptorSetLayout;
		std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings;

		friend class VulkanDescriptorWriter;
	};

	class VulkanDescriptorPool
	{
	public:
		class Builder
		{
		public:
			Builder() {}

			Builder& AddPoolSize(VkDescriptorType descriptorType, uint32_t count);
			Builder& SetPoolFlags(VkDescriptorPoolCreateFlags flags);
			Builder& SetMaxSets(uint32_t count);
			std::unique_ptr<VulkanDescriptorPool> Build() const;
		private:
			std::vector<VkDescriptorPoolSize> m_PoolSizes{};
			uint32_t m_MaxSets = 1000;
			VkDescriptorPoolCreateFlags m_PoolFlags = 0;
		};

		VulkanDescriptorPool(uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes);
		~VulkanDescriptorPool();

		bool AllocateDescriptor(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;
		void FreeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;
		void ResetPool();
	private:
		VkDescriptorPool m_DescriptorPool;

		friend class VulkanDescriptorWriter;
	};

	class VulkanDescriptorWriter
	{
	public:
		VulkanDescriptorWriter(VulkanDescriptorSetLayout& setLayout, VulkanDescriptorPool& pool);

		VulkanDescriptorWriter& WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
		VulkanDescriptorWriter& WriteImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

		bool Build(VkDescriptorSet& set);
		void Overwrite(VkDescriptorSet& set);
	private:
		VulkanDescriptorSetLayout& m_SetLayout;
		VulkanDescriptorPool& m_Pool;
		std::vector<VkWriteDescriptorSet> m_Writes;
	};

}