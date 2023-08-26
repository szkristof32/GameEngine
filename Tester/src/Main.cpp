#include <WhizzEngine/EntryPoint.h>

#include "TestLayer.h"

class Tester : public WhizzEngine::Application
{
public:
	Tester()
	{
		PushLayer(new TestLayer());
	}

	~Tester()
	{
	}
};

WhizzEngine::Application* WhizzEngine::CreateApplication(int argc, char** argv)
{
	return new Tester();
}