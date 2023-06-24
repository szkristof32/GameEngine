#include "pch.h"
#include "Buffers.h"

#include "Platform/OpenGL/OpenGLBuffers.h"

namespace WhizzEngine {

	std::shared_ptr<VertexBuffer> VertexBuffer::Create()
	{
		return std::make_shared<OpenGLVertexBuffer>();
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create()
	{
		return std::make_shared<OpenGLIndexBuffer>();
	}

}