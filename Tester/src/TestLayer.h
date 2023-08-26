#pragma once

#include <WhizzEngine/Core/Layer.h>
#include <WhizzEngine/Rendering/Pipeline.h>
#include <WhizzEngine/Rendering/VertexArray.h>

#include <glm/glm.hpp>
#include <memory>

class TestLayer : public WhizzEngine::Layer
{
public:
	TestLayer();
	~TestLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(WhizzEngine::Timestep ts) override;
	virtual void OnRender() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(std::shared_ptr<WhizzEngine::Event> event) override;
private:
	std::shared_ptr<WhizzEngine::Pipeline> m_Pipeline;
	std::shared_ptr<WhizzEngine::VertexArray> m_VertexArray;
	std::shared_ptr<WhizzEngine::UniformBuffer> m_ProjectionUniformBuffer;
	glm::mat4 m_Projection;
};