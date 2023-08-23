#pragma once

#include "WhizzEngine/Core/Window.h"
#include "WhizzEngine/Events/Event.h"
#include "WhizzEngine/Events/ApplicationEvent.h"
#include "WhizzEngine/Rendering/Context.h"
#include "WhizzEngine/Rendering/RendererAPI.h"

namespace WhizzEngine {

	class Engine
	{
	public:
		static void Init();
		static void Run();
		static void CleanUp();

		static std::shared_ptr<Window> GetWindow();
		static std::shared_ptr<GraphicsContext> GetContext();
		static std::shared_ptr<RendererAPI> GetRendererAPI();
	private:
		static void OnEvent(std::shared_ptr<Event> event);
		static bool OnWindowClose(WindowCloseEvent& event);
	};

}