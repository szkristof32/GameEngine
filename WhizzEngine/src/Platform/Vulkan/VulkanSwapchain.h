#pragma once

#include "WhizzEngine/Rendering/Swapchain.h"

#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

namespace WhizzEngine {

	class VulkanSwapchain : public Swapchain
	{
	public:
		VulkanSwapchain();
		VulkanSwapchain(std::shared_ptr<Swapchain> previous);
		virtual ~VulkanSwapchain();

		VkFramebuffer GetFramebuffer(int index) { return m_SwapchainFramebuffers[index]; }
		VkRenderPass GetRenderPass() { return m_RenderPass; }
		VkImageView GetImageView(int index) { return m_SwapchainImageViews[index]; }
		size_t GetImageCount() { return m_SwapchainImages.size(); }
		VkFormat GetSwapchainImageFormat() { return m_SwapchainImageFormat; }
		VkExtent2D GetSwapchainExtent() { return m_SwapchainExtent; }
		uint32_t GetWidth() { return m_SwapchainExtent.width; }
		uint32_t GetHeight() { return m_SwapchainExtent.height; }

		float ExtentAspectRatio()
		{
			return (float)m_SwapchainExtent.width / (float)m_SwapchainExtent.height;
		}
		VkFormat FindDepthFormat();

		VkResult AcquireNextImage(uint32_t* imageIndex);
		VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

		bool CompareSwapFormats(std::shared_ptr<Swapchain> swapchain) const;
	private:
		void Init();
		void CreateSwapchain();
		void CreateImageViews();
		void CreateDepthResources();
		void CreateRenderPass();
		void CreateFramebuffers();
		void CreateSyncObjects();

		// Helper functions
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	private:
		VkFormat m_SwapchainImageFormat;
		VkFormat m_SwapchainDepthFormat;
		VkExtent2D m_SwapchainExtent;

		std::vector<VkFramebuffer> m_SwapchainFramebuffers;
		VkRenderPass m_RenderPass;

		std::vector<VkImage> m_DepthImages;
		std::vector<VkImageView> m_DepthImageViews;
		std::vector<VmaAllocation> m_DepthImageMemorys;
		std::vector<VkImage> m_SwapchainImages;
		std::vector<VkImageView> m_SwapchainImageViews;

		VkSwapchainKHR m_Swapchain;
		std::shared_ptr<VulkanSwapchain> m_OldSwapchain;

		std::vector<VkSemaphore> m_ImageAvailableSemaphores;
		std::vector<VkSemaphore> m_RenderFinishedSemaphores;
		std::vector<VkFence> m_InFlightFences;
		std::vector<VkFence> m_ImagesInFlight;
		size_t m_CurrentFrame = 0;
	};

}