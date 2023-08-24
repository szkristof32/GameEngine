#include "pch.h"
#include "VulkanCommandBuffer.h"

#include "WhizzEngine/Core/Engine.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace WhizzEngine {

	VulkanCommandBuffer::VulkanCommandBuffer(VkCommandPool commandPool)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		WZ_CORE_ASSERT(vkAllocateCommandBuffers(Engine::GetContext()->As<VulkanContext>(), &allocInfo, &m_CommandBuffer) == VK_SUCCESS, "Failed to allocate command buffer!");
	}

	void VulkanCommandBuffer::Reset() const
	{
		vkDeviceWaitIdle(Engine::GetContext()->As<VulkanContext>());
		vkResetCommandBuffer(m_CommandBuffer, 0);
	}

	void VulkanCommandBuffer::Begin() const
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pInheritanceInfo = nullptr;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		WZ_CORE_ASSERT(vkBeginCommandBuffer(m_CommandBuffer, &beginInfo) == VK_SUCCESS, "Failed to begin command buffer recording!");
	}

	void VulkanCommandBuffer::End() const
	{
		WZ_CORE_ASSERT(vkEndCommandBuffer(m_CommandBuffer) == VK_SUCCESS, "Failed to record command buffer!");
	}

	void VulkanCommandBuffer::Submit(const SubmitInfo& submit) const
	{
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = (uint32_t)submit.WaitSemaphores.size();
		submitInfo.pWaitSemaphores = submit.WaitSemaphores.data();
		submitInfo.pWaitDstStageMask = submit.WaitStages.data();
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffer;
		submitInfo.signalSemaphoreCount = (uint32_t)submit.SignalSemaphores.size();
		submitInfo.pSignalSemaphores = submit.SignalSemaphores.data();

		WZ_CORE_ASSERT(vkQueueSubmit(Engine::GetContext()->As<VulkanContext>(), 1, &submitInfo, submit.Fence) == VK_SUCCESS, "Failed to submit draw command buffer!");
	}

	VulkanCommandPool::VulkanCommandPool()
	{
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = Engine::GetContext()->As<VulkanContext>().FindPhysicalQueueFamilies().GraphicsFamily;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		WZ_CORE_ASSERT(vkCreateCommandPool(Engine::GetContext()->As<VulkanContext>(), &poolInfo, nullptr, &m_CommandPool) == VK_SUCCESS, "Failed to create command pool!");
	}

	VulkanCommandPool::~VulkanCommandPool()
	{
		vkDestroyCommandPool(Engine::GetContext()->As<VulkanContext>(), m_CommandPool, nullptr);
	}

	VulkanCommandBuffer VulkanCommandPool::AllocateBuffer()
	{
		VulkanCommandBuffer cmd(m_CommandPool);
		return cmd;
	}

}