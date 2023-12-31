#pragma once

#include "WhizzEngine/Rendering/RendererAPI.h"

#include "WhizzEngine/Events/ApplicationEvent.h"

namespace WhizzEngine {

	class VulkanRendererAPI : public RendererAPI
	{
	public:
		VulkanRendererAPI();
		~VulkanRendererAPI();

		virtual void Clear() const override;
		virtual void BindPipeline(std::shared_ptr<Pipeline> pipeline) override;
		virtual void Draw(uint32_t vertexCount) override;
		virtual void DrawIndexed(std::shared_ptr<VertexArray> vertexArray) override;
	private:
		void OnEvent(std::shared_ptr<Event> event);
		bool OnWindowResize(WindowResizeEvent& event);
	private:
		std::shared_ptr<Pipeline> m_CurrentlyBoundPipeline = nullptr;
	};

}