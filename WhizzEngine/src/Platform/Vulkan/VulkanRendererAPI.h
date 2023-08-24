#pragma once

#include "WhizzEngine/Rendering/RendererAPI.h"
#include "WhizzEngine/Rendering/Swapchain.h"
#include "Platform/Vulkan/VulkanCommandBuffer.h"

#include "WhizzEngine/Events/ApplicationEvent.h"

#include <glm/glm.hpp>

namespace WhizzEngine {

	class VulkanRendererAPI : public RendererAPI
	{
	public:
		VulkanRendererAPI();
		~VulkanRendererAPI();

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) override;
		virtual void BindPipeline(std::shared_ptr<Pipeline> pipeline) override;
		virtual void Draw(uint32_t vertexCount) override;
		virtual void DrawIndexed(std::shared_ptr<VertexArray> vertexArray) override;

		std::shared_ptr<Swapchain> GetSwapchain() const { return m_Swapchain; }
		std::shared_ptr<VulkanCommandPool> GetCommandPool() const { return m_CommandPool; }
		const VulkanCommandBuffer& GetCurrentCommandBuffer() const { return m_CommandBuffers[m_CurrentFrameIndex]; }
	private:
		void OnEvent(std::shared_ptr<Event> event);
		bool OnWindowResize(WindowResizeEvent& event);

		void RecreateSwapchain();
		void BeginRenderPass();
	private:
		std::shared_ptr<Pipeline> m_CurrentlyBoundPipeline = nullptr;
		std::shared_ptr<Swapchain> m_Swapchain;

		std::shared_ptr<VulkanCommandPool> m_CommandPool;
		std::vector<VulkanCommandBuffer> m_CommandBuffers;
		glm::vec4 m_ClearColour{};

		uint32_t m_CurrentImageIndex = 0;
		uint32_t m_CurrentFrameIndex = 0;
		bool m_IsFrameStarted = false;
	};

}