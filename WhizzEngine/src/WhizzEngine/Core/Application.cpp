#include "pch.h"
#include "Application.h"

#include "WhizzEngine/Core/Window.h"
#include "WhizzEngine/Events/EventBus.h"

#include "WhizzEngine/Rendering/VertexArray.h"

namespace WhizzEngine {

	Application* Application::s_Instance = nullptr;

	static std::shared_ptr<Pipeline> pipeline;
	static std::shared_ptr<VertexArray> vao;
	static std::shared_ptr<VertexBuffer> vbo;
	static std::shared_ptr<IndexBuffer> ibo;

	Application::Application()
	{
		WZ_CORE_ASSERT(!s_Instance, "Cannot have more than one application instance at the same time!");
		s_Instance = this;

		m_Window = Window::Create(1600, 900, "WhizzEngine");
		m_Context = GraphicsContext::Create(m_Window->GetNativeHandle());
		m_RendererAPI = RendererAPI::Create();

		EventBus::RegisterHandler("Engine", WZ_BIND_EVENT_FN(OnEvent));

		// TOOD: test initialisation
		AttributeLayout attribLayout = { { DataType::Float3 } };
		PipelineInfo pipelineInfo{};
		pipelineInfo.ShaderPath = "assets/shaders/testShader.glsl";
		pipelineInfo.AttribLayout = attribLayout;
		pipeline = Pipeline::Create(pipelineInfo);

		float vertices[] = {
			-0.5f, 0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f
		};
		int indices[] = {
			0, 1, 2,
			2, 1, 3
		};

		vao = VertexArray::Create();
		vao->Bind();
		
		vbo = VertexBuffer::Create();
		vbo->Bind();
		vbo->AllocateData(sizeof(vertices));
		vbo->SetData(sizeof(vertices), vertices);
		vao->AddVertexBuffer(vbo, attribLayout);
		vbo->Unbind();

		ibo = IndexBuffer::Create();
		ibo->Bind();
		ibo->AllocateData(sizeof(indices));
		ibo->SetData(sizeof(indices), indices);
		vao->SetIndexBuffer(ibo);
		ibo->Unbind();

		vao->Unbind();
	}

	Application::~Application()
	{
		m_Context->WaitForIdle();

		// TODO: remove
		vao.reset();
		vbo.reset();
		ibo.reset();
		pipeline.reset();

		EventBus::CleanUp();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->Update();
			EventBus::Update();

			m_RendererAPI->BeginFrame();
			m_RendererAPI->Clear(0.82f, 0.04f, 0.04f, 1.0f);

			m_RendererAPI->BindPipeline(pipeline);
			m_RendererAPI->DrawIndexed(vao);

			m_RendererAPI->EndFrame();

			m_Context->Swap();
		}
	}

	void Application::OnEvent(std::shared_ptr<Event> event)
	{
		EventDispatcher dispatcher(*event);
		dispatcher.Dispatch<WindowCloseEvent>(WZ_BIND_EVENT_FN(OnWindowClose));
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

}