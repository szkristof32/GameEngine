#include "pch.h"
#include "VulkanRendererAPI.h"

#include "WhizzEngine/Core/Engine.h"
#include "WhizzEngine/Events/EventBus.h"

#include "Platform/Vulkan/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/VulkanPipeline.h"

#include <vulkan/vulkan.h>

namespace WhizzEngine {

	VulkanRendererAPI::VulkanRendererAPI()
	{
		EventBus::RegisterHandler("VulkanRendererAPI", WZ_BIND_EVENT_FN(OnEvent));

		m_Swapchain = Swapchain::Create();
		m_CommandPool = std::make_shared<VulkanCommandPool>();
		for (size_t i = 0; i < Swapchain::MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_CommandBuffers.emplace_back(m_CommandPool->AllocateBuffer());
		}
	}

	VulkanRendererAPI::~VulkanRendererAPI()
	{
	}

	void VulkanRendererAPI::BeginFrame()
	{
		WZ_CORE_ASSERT(!m_IsFrameStarted, "Cannot begin new frame while another is already in progress!");

		auto result = m_Swapchain->As<VulkanSwapchain>().AcquireNextImage(&m_CurrentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapchain();
			return;
		}

		WZ_CORE_ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR, "Failed to acquire swapchain image!");
		m_IsFrameStarted = true;
		m_CommandBuffers[m_CurrentFrameIndex].Begin();
		BeginRenderPass();
	}

	void VulkanRendererAPI::EndFrame()
	{
		WZ_CORE_ASSERT(m_IsFrameStarted, "Cannot end the frame while frame is not in progress!");

		vkCmdEndRenderPass(m_CommandBuffers[m_CurrentFrameIndex]);
		m_CommandBuffers[m_CurrentFrameIndex].End();

		auto result = m_Swapchain->As<VulkanSwapchain>().SubmitCommandBuffers((const VkCommandBuffer*)&m_CommandBuffers[m_CurrentFrameIndex], &m_CurrentImageIndex);

		if (Engine::IsClosing())
			return;

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			RecreateSwapchain();
		}
		else
		{
			WZ_CORE_ASSERT(result == VK_SUCCESS, "Failed to present swapchain image!");
		}

		m_IsFrameStarted = false;
		m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % Swapchain::MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanRendererAPI::Clear(float r, float g, float b, float a)
	{
		m_ClearColour = glm::vec4{ r, g, b, a };
	}

	void VulkanRendererAPI::BindPipeline(std::shared_ptr<Pipeline> pipeline)
	{
		m_CurrentlyBoundPipeline = pipeline;
		vkCmdBindPipeline(m_CommandBuffers[m_CurrentFrameIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->As<VulkanPipeline>());
	}

	void VulkanRendererAPI::Draw(uint32_t vertexCount)
	{
		vkCmdDraw(m_CommandBuffers[m_CurrentFrameIndex], vertexCount, 1, 0, 0);
	}

	void VulkanRendererAPI::DrawIndexed(std::shared_ptr<VertexArray> vertexArray)
	{
		for (auto vertexBuffer : vertexArray->GetVertexBuffers())
		{
			vertexBuffer->Bind();
		}
		vertexArray->GetIndexBuffer()->Bind();
		vkCmdDrawIndexed(m_CommandBuffers[m_CurrentFrameIndex], vertexArray->GetIndexBuffer()->GetDataSize() / sizeof(uint32_t), 1, 0, 0, 0);
	}

	void VulkanRendererAPI::OnEvent(std::shared_ptr<Event> event)
	{
		EventDispatcher dispatcher(*event);
		dispatcher.Dispatch<WindowResizeEvent>(WZ_BIND_EVENT_FN(OnWindowResize));
	}

	bool VulkanRendererAPI::OnWindowResize(WindowResizeEvent& event)
	{
		RecreateSwapchain();
		return false;
	}

	void VulkanRendererAPI::RecreateSwapchain()
	{
		auto& context = Engine::GetContext()->As<VulkanContext>();
		auto& window = Engine::GetWindow();

		VkExtent2D extent = { window->GetWidth(), window->GetHeight() };
		while (extent.width == 0 || extent.height == 0)
		{
			extent = { window->GetWidth(), window->GetHeight() };
			window->WaitForEvents();
		}

		vkDeviceWaitIdle(context);
		if (!m_Swapchain)
		{
			m_Swapchain = Swapchain::Create();
		}
		else
		{
			std::shared_ptr<Swapchain> oldSwapchain = std::move(m_Swapchain);
			m_Swapchain = Swapchain::Create(oldSwapchain);
			WZ_CORE_ASSERT(oldSwapchain->As<VulkanSwapchain>().CompareSwapFormats(m_Swapchain), "Swapchain image (or depth) format has changed!");
		}
	}

	void VulkanRendererAPI::BeginRenderPass()
	{
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_Swapchain->As<VulkanSwapchain>().GetRenderPass();
		renderPassInfo.framebuffer = m_Swapchain->As<VulkanSwapchain>().GetFramebuffer(m_CurrentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_Swapchain->As<VulkanSwapchain>().GetSwapchainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { m_ClearColour.r, m_ClearColour.g, m_ClearColour.b, m_ClearColour.a };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = (uint32_t)clearValues.size();
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(m_CommandBuffers[m_CurrentFrameIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)m_Swapchain->As<VulkanSwapchain>().GetSwapchainExtent().width;
		viewport.height = (float)m_Swapchain->As<VulkanSwapchain>().GetSwapchainExtent().height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor = { { 0, 0 }, m_Swapchain->As<VulkanSwapchain>().GetSwapchainExtent() };
		vkCmdSetViewport(m_CommandBuffers[m_CurrentFrameIndex], 0, 1, &viewport);
		vkCmdSetScissor(m_CommandBuffers[m_CurrentFrameIndex], 0, 1, &scissor);
	}

}