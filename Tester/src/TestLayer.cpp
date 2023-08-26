#include "TestLayer.h"

#include <WhizzEngine/Core/Application.h>

using namespace WhizzEngine;

TestLayer::TestLayer()
{
	AttributeLayout attributeLayout = {
		DataType::Float3
	};

	m_ProjectionUniformBuffer = UniformBuffer::Create(sizeof(glm::mat4), 0);

	PipelineInfo pipelineInfo{};
	pipelineInfo.AttribLayout = attributeLayout;
	pipelineInfo.ShaderPath = "assets/shaders/testShader.glsl";
	pipelineInfo.UniformBuffers = { m_ProjectionUniformBuffer };

	m_Pipeline = Pipeline::Create(pipelineInfo);

	m_Projection = glm::perspectiveFov(70.0f, (float)Application::Get().GetWindow()->GetWidth(), (float)Application::Get().GetWindow()->GetHeight(), 0.01f, 100.f);

	float vertexData[] = {
		-0.5f, 0.5f, -3.0f,
		-0.5f, -0.5f, -3.0f,
		0.5f, 0.5f, -3.0f,
		0.5f, -0.5f, -3.0f
	};
	uint32_t indexData[] = {
		0, 1, 2,
		2, 1, 3
	};

	m_VertexArray = VertexArray::Create();
	m_VertexArray->Bind();

	auto vertexBuffer = VertexBuffer::Create();
	vertexBuffer->Bind();
	vertexBuffer->SetData(sizeof(vertexData), vertexData);
	m_VertexArray->AddVertexBuffer(vertexBuffer, attributeLayout);
	vertexBuffer->Unbind();

	auto indexBuffer = IndexBuffer::Create();
	indexBuffer->Bind();
	indexBuffer->SetData(sizeof(indexData), indexData);
	m_VertexArray->SetIndexBuffer(indexBuffer);
	indexBuffer->Unbind();

	m_VertexArray->Unbind();
}

TestLayer::~TestLayer()
{
}

void TestLayer::OnAttach()
{
}

void TestLayer::OnDetach()
{
}

void TestLayer::OnUpdate(Timestep ts)
{
}

void TestLayer::OnRender()
{
	Application::Get().GetRendererAPI()->BeginFrame();
	Application::Get().GetRendererAPI()->Clear(0.8f, 0.04f, 0.05f, 1.0f);

	Application::Get().GetRendererAPI()->BindPipeline(m_Pipeline);
	m_ProjectionUniformBuffer->SetData(sizeof(glm::mat4), &m_Projection);
	Application::Get().GetRendererAPI()->DrawIndexed(m_VertexArray);

	Application::Get().GetRendererAPI()->EndFrame();
}

void TestLayer::OnImGuiRender()
{
}

void TestLayer::OnEvent(std::shared_ptr<Event> event)
{
}