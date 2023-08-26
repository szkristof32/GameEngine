#pragma once

#include <memory>

namespace WhizzEngine {

	class Swapchain
	{
	public:
		static constexpr int MAX_FRAMES_IN_FLIGHT = 2;
		
		virtual ~Swapchain() = default;

		WZ_CAST_TO(Swapchain);

		static std::shared_ptr<Swapchain> Create();
		static std::shared_ptr<Swapchain> Create(std::shared_ptr<Swapchain> previous);
	};

}