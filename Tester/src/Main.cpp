#include <WhizzEngine/EntryPoint.h>

class Tester : public WhizzEngine::Application
{
public:
	Tester()
	{
	}

	~Tester()
	{
	}
};

WhizzEngine::Application* WhizzEngine::CreateApplication(int argc, char** argv)
{
	return new Tester();
}