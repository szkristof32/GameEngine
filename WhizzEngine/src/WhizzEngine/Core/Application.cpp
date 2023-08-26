#include "pch.h"
#include "Application.h"

#include "WhizzEngine/Events/EventBus.h"

namespace WhizzEngine {

	static constexpr float MS_PER_UPDATE = 1.0f / 60.0f;

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		WZ_CORE_ASSERT(!s_Instance, "Cannot have more than one application instance at the same time!");
		s_Instance = this;

		m_Window = Window::Create(1600, 900, "WhizzEngine");
		m_Context = GraphicsContext::Create(m_Window->GetNativeHandle());
		m_RendererAPI = RendererAPI::Create();

		EventBus::RegisterHandler("Engine", WZ_BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{
		m_Context->WaitForIdle();

		EventBus::CleanUp();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float currentTime = Time::GetTime();
			Timestep delta = currentTime - m_LastFrameTime;
			m_LastFrameTime = currentTime;
			m_Lag += delta;

			while (m_Lag >= MS_PER_UPDATE)
			{
				m_Window->Update();
				EventBus::Update();

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(delta);

				m_Lag -= MS_PER_UPDATE;
			}

			for (Layer* layer : m_LayerStack)
				layer->OnRender();

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