#pragma once

#include "WhizzEngine/Core/Core.h"

#include <glm/glm.hpp>
#include <string>
#include <memory>

namespace WhizzEngine {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Start() = 0;
		virtual void Stop() = 0;

		WZ_CAST_TO(Shader);

		static std::shared_ptr<Shader> Create(const std::string& filepath);
	};

}