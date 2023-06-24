#include "pch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace WhizzEngine {

	std::shared_ptr<Shader> Shader::Create(const std::string& filepath)
	{
		return std::make_shared<OpenGLShader>(filepath);
	}

}