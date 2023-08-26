#pragma once

#include <vulkan/vulkan.h>

namespace WhizzEngine {

	struct SubmitInfo
	{
		std::vector<VkSemaphore> WaitSemaphores{};
		std::vector<VkSemaphore> SignalSemaphores{};
		VkFence Fence = nullptr;
		std::vector<VkPipelineStageFlags> WaitStages{};
	};

	class VulkanCommandBuffer
	{
	public:
		~VulkanCommandBuffer() = default;

		void Reset() const;
		void Begin() const;
		void End() const;
		void Submit(const SubmitInfo& submitInfo = SubmitInfo{}) const;

		operator VkCommandBuffer() const { return m_CommandBuffer; }
	protected:
		VulkanCommandBuffer(VkCommandPool commandPool);
	private:
		VkCommandBuffer m_CommandBuffer;

		friend class VulkanCommandPool;
	};

	class VulkanCommandPool
	{
	public:
		VulkanCommandPool();
		~VulkanCommandPool();

		VulkanCommandBuffer AllocateBuffer();
	private:
		VkCommandPool m_CommandPool;
	};

}