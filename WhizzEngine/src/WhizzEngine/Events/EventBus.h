#pragma once

#include "WhizzEngine/Events/Event.h"

#include <string>
#include <queue>
#include <unordered_map>
#include <memory>
#include <functional>

namespace WhizzEngine {

	using EventHandler = std::function<void(std::shared_ptr<Event>)>;

	class EventBus
	{
	public:
		static void Update();
		static void CleanUp();

		static void PostEvent(std::shared_ptr<Event> event) { s_EventQueue.push(event); }
		static void RegisterHandler(const std::string& name, EventHandler handler);
		static void UnregisterHandler(const std::string& name);
	private:
		static std::queue<std::shared_ptr<Event>> s_EventQueue;
		static std::unordered_map<std::string, EventHandler> s_EventHandlers;
	};

}