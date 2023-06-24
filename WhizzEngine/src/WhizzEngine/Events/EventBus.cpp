#include "pch.h"
#include "EventBus.h"

namespace WhizzEngine {

	std::queue<std::shared_ptr<Event>> EventBus::s_EventQueue;
	std::unordered_map<std::string, EventHandler> EventBus::s_EventHandlers;

	void EventBus::Update()
	{
		uint32_t count = (uint32_t)std::ceil(s_EventQueue.size() * 0.75);
		while (count--)
		{
			std::shared_ptr<Event> event = s_EventQueue.front();
			for (auto&& [name, handler] : s_EventHandlers)
				handler(event);
			s_EventQueue.pop();
		}
	}

	void EventBus::CleanUp()
	{
		while (s_EventQueue.size())
		{
			std::shared_ptr<Event> event = s_EventQueue.front();
			for (auto&& [name, handler] : s_EventHandlers)
				handler(event);
			s_EventQueue.pop();
		}
	}

	void EventBus::RegisterHandler(const std::string& name, EventHandler handler)
	{
		if (s_EventHandlers.count(name))
		{
			WZ_CORE_WARN("Event handler with name {0} already registered!", name);
			return;
		}
		s_EventHandlers.insert({ name, handler });
	}

	void EventBus::UnregisterHandler(const std::string& name)
	{
		if (!s_EventHandlers.count(name))
		{
			WZ_CORE_WARN("Couldn't find event handler with name {0}!", name);
			return;
		}
		s_EventHandlers.erase(name);
	}

}