#include "pch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace WhizzEngine {

	std::shared_ptr<RendererAPI> RendererAPI::Create()
	{
		return std::make_shared<OpenGLRendererAPI>();
	}

}