#pragma once

#include "WhizzEngine/Core/Window.h"
#include "WhizzEngine/Events/Event.h"
#include "WhizzEngine/Events/ApplicationEvent.h"
#include "WhizzEngine/Rendering/Context.h"
#include "WhizzEngine/Rendering/RendererAPI.h"

namespace WhizzEngine {

	class Application
	{
	public:
		Application();
		~Application();

		void Run();

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

		static Application* s_Instance;
	};

	Application* CreateApplication(int argc, char** argv);

}