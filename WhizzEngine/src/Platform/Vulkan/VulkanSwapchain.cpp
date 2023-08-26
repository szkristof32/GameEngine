#include "pch.h"
#include "VulkanSwapchain.h"

#include "WhizzEngine/Core/Application.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace WhizzEngine {

	VulkanSwapchain::VulkanSwapchain()
	{
		Init();
	}

	VulkanSwapchain::VulkanSwapchain(std::shared_ptr<Swapchain> previous)
		: m_OldSwapchain(std::static_pointer_cast<VulkanSwapchain>(previous))
	{
		Init();
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		auto& context = Application::Get().GetContext()->As<VulkanContext>();

		for (auto imageView : m_SwapchainImageViews)
		{
			vkDestroyImageView(context, imageView, nullptr);
		}
		m_SwapchainImageViews.clear();

		if (m_Swapchain)
		{
			vkDestroySwapchainKHR(context, m_Swapchain, nullptr);
			m_Swapchain = nullptr;
		}

		for (size_t i = 0; i < m_DepthImages.size(); i++)
		{
			vkDestroyImageView(context, m_DepthImageViews[i], nullptr);
			vmaDestroyImage(context, m_DepthImages[i], m_DepthImageMemorys[i]);
		}

		for (auto framebuffer : m_SwapchainFramebuffers)
		{
			vkDestroyFramebuffer(context, framebuffer, nullptr);
		}

		vkDestroyRenderPass(context, m_RenderPass, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(context, m_RenderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(context, m_ImageAvailableSemaphores[i], nullptr);
			vkDestroyFence(context, m_InFlightFences[i], nullptr);
		}
	}

	VkFormat VulkanSwapchain::FindDepthFormat()
	{
		std::vector<VkFormat> formats = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
		for (VkFormat format : formats)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(Application::Get().GetContext()->As<VulkanContext>(), format, &props);

			if ((props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) == VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
			{
				return format;
			}
		}
		WZ_CORE_ASSERT(false, "Failed to find supported format!");
		return VK_FORMAT_MAX_ENUM;
	}

	VkResult VulkanSwapchain::AcquireNextImage(uint32_t* imageIndex)
	{
		auto& context = Application::Get().GetContext()->As<VulkanContext>();

		vkWaitForFences(context, 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
		VkResult result = vkAcquireNextImageKHR(context, m_Swapchain, std::numeric_limits<uint64_t>::max(), m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, imageIndex);
		return result;
	}

	VkResult VulkanSwapchain::SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex)
	{
		auto& context = Application::Get().GetContext()->As<VulkanContext>();
		if (m_ImagesInFlight[*imageIndex] != VK_NULL_HANDLE)
		{
			vkWaitForFences(context, 1, &m_ImagesInFlight[*imageIndex], VK_TRUE, std::numeric_limits<uint64_t>::max());
		}
		m_ImagesInFlight[*imageIndex] = m_InFlightFences[m_CurrentFrame];

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = buffers;
		VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(context, 1, &m_InFlightFences[m_CurrentFrame]);
		WZ_CORE_ASSERT(vkQueueSubmit(context, 1, &submitInfo, m_InFlightFences[m_CurrentFrame]) == VK_SUCCESS, "Failed to submit draw command buffer!");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_Swapchain;
		presentInfo.pImageIndices = imageIndex;

		VkResult result = vkQueuePresentKHR(context, &presentInfo);
		m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
		return result;
	}

	bool VulkanSwapchain::CompareSwapFormats(std::shared_ptr<Swapchain> swapchain) const
	{
		VulkanSwapchain& other = swapchain->As<VulkanSwapchain>();
		return other.m_SwapchainDepthFormat == m_SwapchainDepthFormat && other.m_SwapchainImageFormat == m_SwapchainImageFormat;
	}

	void VulkanSwapchain::Init()
	{
		CreateSwapchain();
		CreateImageViews();
		CreateRenderPass();
		CreateDepthResources();
		CreateFramebuffers();
		CreateSyncObjects();
	}

	void VulkanSwapchain::CreateSwapchain()
	{
		auto& context = Application::Get().GetContext()->As<VulkanContext>();
		SwapchainSupportDetails swapchainSupport = context.GetSwapchainSupport();

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapchainSupport.Formats);
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapchainSupport.PresentModes);
		VkExtent2D extent = ChooseSwapExtent(swapchainSupport.Capabilities);

		uint32_t imageCount = swapchainSupport.Capabilities.minImageCount + 1;
		if (swapchainSupport.Capabilities.maxImageCount > 0 && imageCount > swapchainSupport.Capabilities.maxImageCount)
		{
			imageCount = swapchainSupport.Capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR swapchainInfo{};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = context.GetSurface();
		swapchainInfo.minImageCount = imageCount;
		swapchainInfo.imageFormat = surfaceFormat.format;
		swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainInfo.imageExtent = extent;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = context.FindPhysicalQueueFamilies();
		uint32_t queueFamilyIndices[] = { indices.GraphicsFamily, indices.PresentFamily };
		if (indices.GraphicsFamily != indices.PresentFamily)
		{
			swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainInfo.queueFamilyIndexCount = 2;
			swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapchainInfo.queueFamilyIndexCount = 0;
			swapchainInfo.pQueueFamilyIndices = nullptr;
		}

		swapchainInfo.preTransform = swapchainSupport.Capabilities.currentTransform;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.presentMode = presentMode;
		swapchainInfo.clipped = VK_TRUE;
		swapchainInfo.oldSwapchain = !m_OldSwapchain ? VK_NULL_HANDLE : m_OldSwapchain->m_Swapchain;

		WZ_CORE_ASSERT(vkCreateSwapchainKHR(context, &swapchainInfo, nullptr, &m_Swapchain) == VK_SUCCESS, "Failed to create swapchain!");

		vkGetSwapchainImagesKHR(context, m_Swapchain, &imageCount, nullptr);
		m_SwapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(context, m_Swapchain, &imageCount, m_SwapchainImages.data());

		m_SwapchainImageFormat = surfaceFormat.format;
		m_SwapchainExtent = extent;
	}

	void VulkanSwapchain::CreateImageViews()
	{
		m_SwapchainImageViews.resize(m_SwapchainImages.size());
		for (size_t i = 0; i < m_SwapchainImages.size(); i++)
		{
			VkImageViewCreateInfo viewInfo{};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = m_SwapchainImages[i];
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = m_SwapchainImageFormat;
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = 1;

			WZ_CORE_ASSERT(vkCreateImageView(Application::Get().GetContext()->As<VulkanContext>(), &viewInfo, nullptr, &m_SwapchainImageViews[i]) == VK_SUCCESS, "Failed to create texture image view!");
		}
	}

	void VulkanSwapchain::CreateDepthResources()
	{
		auto& context = Application::Get().GetContext()->As<VulkanContext>();

		VkFormat depthFormat = FindDepthFormat();
		m_SwapchainDepthFormat = depthFormat;
		VkExtent2D swapChainExtent = GetSwapchainExtent();

		m_DepthImages.resize(GetImageCount());
		m_DepthImageMemorys.resize(GetImageCount());
		m_DepthImageViews.resize(GetImageCount());

		for (int i = 0; i < m_DepthImages.size(); i++)
		{
			VkImageCreateInfo imageInfo{};
			imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageInfo.imageType = VK_IMAGE_TYPE_2D;
			imageInfo.extent.width = swapChainExtent.width;
			imageInfo.extent.height = swapChainExtent.height;
			imageInfo.extent.depth = 1;
			imageInfo.mipLevels = 1;
			imageInfo.arrayLayers = 1;
			imageInfo.format = depthFormat;
			imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			imageInfo.flags = 0;

			VmaAllocationCreateInfo allocInfo{};
			allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
			allocInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

			WZ_CORE_ASSERT(vmaCreateImage(context, &imageInfo, &allocInfo, &m_DepthImages[i], &m_DepthImageMemorys[i], nullptr) == VK_SUCCESS, "Failed to create depth images!");

			VkImageViewCreateInfo viewInfo{};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = m_DepthImages[i];
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = depthFormat;
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = 1;

			WZ_CORE_ASSERT(vkCreateImageView(context, &viewInfo, nullptr, &m_DepthImageViews[i]) == VK_SUCCESS, "Failed to create texture image view!");
		}
	}

	void VulkanSwapchain::CreateRenderPass()
	{
		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = FindDepthFormat();
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = GetSwapchainImageFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.srcAccessMask = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstSubpass = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		WZ_CORE_ASSERT(vkCreateRenderPass(Application::Get().GetContext()->As<VulkanContext>(), &renderPassInfo, nullptr, &m_RenderPass) == VK_SUCCESS, "Failed to create render pass!");
	}

	void VulkanSwapchain::CreateFramebuffers()
	{
		m_SwapchainFramebuffers.resize(GetImageCount());
		for (size_t i = 0; i < GetImageCount(); i++)
		{
			std::array<VkImageView, 2> attachments = { m_SwapchainImageViews[i], m_DepthImageViews[i] };
			VkExtent2D swapchainExtent = GetSwapchainExtent();

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = m_RenderPass;
			framebufferInfo.attachmentCount = (uint32_t)attachments.size();
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = swapchainExtent.width;
			framebufferInfo.height = swapchainExtent.height;
			framebufferInfo.layers = 1;

			WZ_CORE_ASSERT(vkCreateFramebuffer(Application::Get().GetContext()->As<VulkanContext>(), &framebufferInfo, nullptr, &m_SwapchainFramebuffers[i]) == VK_SUCCESS, "Failed to create framebuffer!");
		}
	}

	void VulkanSwapchain::CreateSyncObjects()
	{
		auto& context = Application::Get().GetContext()->As<VulkanContext>();

		m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		m_ImagesInFlight.resize(GetImageCount(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			WZ_CORE_ASSERT(vkCreateSemaphore(context, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) == VK_SUCCESS, "Failed to create synchronisation objects for a frame!");
			WZ_CORE_ASSERT(vkCreateSemaphore(context, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) == VK_SUCCESS, "Failed to create synchronisation objects for a frame!");
			WZ_CORE_ASSERT(vkCreateFence(context, &fenceInfo, nullptr, &m_InFlightFences[i]) == VK_SUCCESS, "Failed to create synchronisation objects for a frame!");
		}
	}

	VkSurfaceFormatKHR VulkanSwapchain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}
		return availableFormats[0];
	}

	VkPresentModeKHR VulkanSwapchain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}
#define WZ_PRESENT_MODE_FIFO 1
#if WZ_PRESENT_MODE_FIFO
		for (const auto& availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
			{
				return availablePresentMode;
			}
		}
#endif
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanSwapchain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			VkExtent2D actualExtent = { Application::Get().GetWindow()->GetWidth(), Application::Get().GetWindow()->GetHeight() };
			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
			return actualExtent;
		}
	}

}