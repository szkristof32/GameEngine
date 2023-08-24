#pragma once

#include "WhizzEngine/Core/Window.h"
#include "WhizzEngine/Events/Event.h"
#include "WhizzEngine/Events/ApplicationEvent.h"
#include "WhizzEngine/Rendering/Context.h"
#include "WhizzEngine/Rendering/RendererAPI.h"
#include "WhizzEngine/Core/LayerStack.h"

namespace WhizzEngine {

	class Application
	{
	public:
		Application();
		~Application();

		void Run();

		void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
		void PushOverlay(Layer* overlay) { m_LayerStack.PushOverlay(overlay); }

		static Application& Get() { return *s_Instance; }

		std::shared_ptr<Window> GetWindow() const { return m_Window; }
		std::shared_ptr<GraphicsContext> GetContext() const { return m_Context; }
		std::shared_ptr<RendererAPI> GetRendererAPI() const { return m_RendererAPI; }
		bool IsClosing() const { return !m_Running; }
	private:
		void OnEvent(std::shared_ptr<Event> event);
		bool OnWindowClose(WindowCloseEvent& event);
	private:
		bool m_Running = true;
		std::shared_ptr<Window> m_Window;
		std::shared_ptr<GraphicsContext> m_Context;
		std::shared_ptr<RendererAPI> m_RendererAPI;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		float m_Lag = 0.0f;

		static Application* s_Instance;
	};

	Application* CreateApplication(int argc, char** argv);

}