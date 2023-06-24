#pragma once

#include "WhizzEngine/Events/Event.h"
#include "WhizzEngine/Events/ApplicationEvent.h"

namespace WhizzEngine {

	class Engine
	{
	public:
		static void Init();
		static void Run();
		static void CleanUp();
	private:
		static void OnEvent(std::shared_ptr<Event> event);
		static bool OnWindowClose(WindowCloseEvent& event);
	};

}