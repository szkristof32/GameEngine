#pragma once

#include "WhizzEngine/Core/Timing.h"
#include "WhizzEngine/Events/Event.h"

namespace WhizzEngine {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate(Timestep ts) = 0;
		virtual void OnImGuiRender() = 0;
		virtual void OnEvent(std::shared_ptr<Event> event) = 0;

		const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}