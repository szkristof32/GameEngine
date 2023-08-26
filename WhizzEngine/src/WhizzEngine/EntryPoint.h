#pragma once

#include "WhizzEngine/Core/Application.h"
#include "WhizzEngine/Core/Log.h"

extern WhizzEngine::Application* WhizzEngine::CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "");

	WhizzEngine::Log::Init();

	auto app = WhizzEngine::CreateApplication(argc, argv);
	app->Run();
	delete app;
}