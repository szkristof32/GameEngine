#include "pch.h"
#include "OpenGLRendererAPI.h"

#include "WhizzEngine/Events/EventBus.h"

#include <glad/glad.h>

namespace WhizzEngine {

	OpenGLRendererAPI::OpenGLRendererAPI()
	{
		EventBus::RegisterHandler("OpenGLRendererAPI", WZ_BIND_EVENT_FN(OnEvent));
	}

	OpenGLRendererAPI::~OpenGLRendererAPI()
	{
		m_CurrentlyBoundPipeline = nullptr;
	}

	void OpenGLRendererAPI::BeginFrame()
	{
	}

	void OpenGLRendererAPI::EndFrame()
	{
	}

	void OpenGLRendererAPI::Clear() const
	{
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::BindPipeline(std::shared_ptr<Pipeline> pipeline)
	{
		m_CurrentlyBoundPipeline = pipeline;
	}

	void OpenGLRendererAPI::Draw(uint32_t vertexCount)
	{
		WZ_CORE_ASSERT(m_CurrentlyBoundPipeline);
		m_CurrentlyBoundPipeline->GetShader()->Start();
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

	void OpenGLRendererAPI::DrawIndexed(std::shared_ptr<VertexArray> vertexArray)
	{
		WZ_CORE_ASSERT(m_CurrentlyBoundPipeline);
		WZ_CORE_ASSERT(m_CurrentlyBoundPipeline->GetAttributeLayout() == vertexArray->GetAttributeLayout(), "Attribute layouts must match!");
		m_CurrentlyBoundPipeline->GetShader()->Start();
		vertexArray->Bind();
		auto size = vertexArray->GetIndexBuffer()->GetDataSize() / sizeof(uint32_t);
		glDrawElements(GL_TRIANGLES, (uint32_t)size, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::OnEvent(std::shared_ptr<Event> event)
	{
		EventDispatcher dispatcher(*event);
		dispatcher.Dispatch<WindowResizeEvent>(WZ_BIND_EVENT_FN(OnWindowResize));
	}

	bool OpenGLRendererAPI::OnWindowResize(WindowResizeEvent& event)
	{
		glViewport(0, 0, event.GetWidth(), event.GetHeight());
		return false;
	}

}