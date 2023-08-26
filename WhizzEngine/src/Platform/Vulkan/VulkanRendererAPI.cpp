#include "pch.h"
#include "VulkanRendererAPI.h"

#include "WhizzEngine/Events/EventBus.h"

#include <vulkan/vulkan.h>

namespace WhizzEngine {

	VulkanRendererAPI::VulkanRendererAPI()
	{
		EventBus::RegisterHandler("VulkanRendererAPI", WZ_BIND_EVENT_FN(OnEvent));
	}

	VulkanRendererAPI::~VulkanRendererAPI()
	{

	}

	void VulkanRendererAPI::Clear() const
	{

	}

	void VulkanRendererAPI::BindPipeline(std::shared_ptr<Pipeline> pipeline)
	{

	}

	void VulkanRendererAPI::Draw(uint32_t vertexCount)
	{

	}

	void VulkanRendererAPI::DrawIndexed(std::shared_ptr<VertexArray> vertexArray)
	{

	}

	void VulkanRendererAPI::OnEvent(std::shared_ptr<Event> event)
	{

	}

	bool VulkanRendererAPI::OnWindowResize(WindowResizeEvent& event)
	{
		return false;
	}

}