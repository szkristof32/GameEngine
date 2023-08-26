#pragma once

#include "WhizzEngine/Rendering/Shader.h"

#include <glm/glm.hpp>
#include <unordered_map>

namespace WhizzEngine {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		virtual ~OpenGLShader();

		virtual void Start() override;
		virtual void Stop() override;
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<uint32_t, std::string> PreProcess(const std::string& source);

		void CompileOrGetOpenGLBinaries();
		void CreateProgram();
		void Reflect(uint32_t stage, const std::vector<uint32_t>& shaderData);
	private:
		uint32_t m_RendererID;
		std::string m_FilePath;
		std::string m_Name;
		bool m_Started = false;

		std::unordered_map<uint32_t, std::vector<uint32_t>> m_OpenGLSPIRV;
		std::unordered_map<uint32_t, std::string> m_OpenGLSourceCode;

		friend class OpenGLUniformBuffer;
	};

}