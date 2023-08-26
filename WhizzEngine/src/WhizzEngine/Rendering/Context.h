#pragma once

#include "WhizzEngine/Core/Core.h"

#include <memory>

namespace WhizzEngine {

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Swap() = 0;
		virtual void WaitForIdle() = 0;

		WZ_CAST_TO(GraphicsContext);

		static std::shared_ptr<GraphicsContext> Create(const void* windowHandle);
	};

}