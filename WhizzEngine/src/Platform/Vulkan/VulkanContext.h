#pragma once

#include "WhizzEngine/Rendering/Context.h"

#if defined(WZ_PLATFORM_WINDOWS)
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

namespace WhizzEngine {

	struct SwapchainSupportDetails
	{
		VkSurfaceCapabilitiesKHR Capabilities;
		std::vector<VkSurfaceFormatKHR> Formats;
		std::vector<VkPresentModeKHR> PresentModes;
	};

	struct QueueFamilyIndices
	{
		uint32_t GraphicsFamily;
		uint32_t PresentFamily;
		bool GraphicsFamilyHasValue = false;
		bool PresentFamilyHasValue = false;
		bool IsComplete() { return GraphicsFamilyHasValue && PresentFamilyHasValue; }
	};

	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext(const void* windowHandle);
		~VulkanContext();

		virtual void Swap() override {};
		virtual void WaitForIdle() override { vkDeviceWaitIdle(m_Device); }

		operator VkInstance() const { return m_Instance; }
		VkInstance GetInstance() const { return m_Instance; }
		operator VkPhysicalDevice() const { return m_PhysicalDevice; }
		VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
		operator VkDevice() const { return m_Device; }
		VkDevice GetDevice() const { return m_Device; }
		operator VkSurfaceKHR() const { return m_Surface; }
		VkSurfaceKHR GetSurface() const { return m_Surface; }
		operator VkQueue() const { return m_GraphicsQueue; }
		VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
		operator VmaAllocator() const { return m_Allocator; }
		VmaAllocator GetAllocator() const { return m_Allocator; }

		SwapchainSupportDetails GetSwapchainSupport() { return QuerySwapchainSupport(m_PhysicalDevice); }
		QueueFamilyIndices FindPhysicalQueueFamilies() { return FindQueueFamilies(m_PhysicalDevice); }
	private:
		void CreateInstance();
		void SetupDebugMessenger();
		void CreateSurface(const void* windowHandle);
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		void CreateAllocator();

		std::vector<const char*> GetRequiredExtensions();
		bool CheckValidationLayerSupport();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void CheckExtensions();
		bool IsDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device);
	public:
		VkPhysicalDeviceProperties GpuProperties;
	private:
		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		VkDevice m_Device;
		VkPhysicalDevice m_PhysicalDevice;
		VkSurfaceKHR m_Surface;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		VmaAllocator m_Allocator;

		const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
		const std::vector<const char*> m_DeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	};

}