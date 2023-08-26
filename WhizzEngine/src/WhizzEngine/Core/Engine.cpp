#include "pch.h"
#include "Engine.h"

#include "WhizzEngine/Core/Window.h"
#include "WhizzEngine/Events/EventBus.h"

#include "WhizzEngine/Rendering/VertexArray.h"

namespace WhizzEngine {

	static bool s_Running = true;
	static std::shared_ptr<Window> s_Window;
	static std::shared_ptr<GraphicsContext> s_Context;
	static std::shared_ptr<RendererAPI> s_RendererAPI;

	static std::shared_ptr<Pipeline> pipeline;
	static std::shared_ptr<VertexArray> vao;

	void Engine::Init()
	{
		s_Window = Window::Create(1600, 900, "FuryEngine");
		s_Context = GraphicsContext::Create(s_Window->GetNativeHandle());
		s_RendererAPI = RendererAPI::Create();

		EventBus::RegisterHandler("Engine", OnEvent);

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
		
		auto vbo = VertexBuffer::Create();
		vbo->Bind();
		vbo->AllocateData(sizeof(vertices));
		vbo->SetData(sizeof(vertices), vertices);
		vao->AddVertexBuffer(vbo, attribLayout);
		vbo->Unbind();

		auto ibo = IndexBuffer::Create();
		ibo->Bind();
		ibo->AllocateData(sizeof(indices));
		ibo->SetData(sizeof(indices), indices);
		vao->SetIndexBuffer(ibo);
		ibo->Unbind();

		vao->Unbind();
	}

	void Engine::Run()
	{
		while (s_Running)
		{
			s_Window->Update();
			EventBus::Update();

			s_RendererAPI->Clear();

			s_RendererAPI->BindPipeline(pipeline);
			s_RendererAPI->DrawIndexed(vao);

			s_Context->Swap();
		}
	}

	void Engine::CleanUp()
	{
		EventBus::CleanUp();

		s_RendererAPI.reset();
		s_Context.reset();
		s_Window.reset();
	}

	std::shared_ptr<GraphicsContext> Engine::GetContext()
	{
		return s_Context;
	}

	std::shared_ptr<RendererAPI> Engine::GetRendererAPI()
	{
		return s_RendererAPI;
	}

	void Engine::OnEvent(std::shared_ptr<Event> event)
	{
		EventDispatcher dispatcher(*event);
		dispatcher.Dispatch<WindowCloseEvent>(OnWindowClose);
	}

	bool Engine::OnWindowClose(WindowCloseEvent& event)
	{
		s_Running = false;
		return true;
	}

}