#pragma once

#include "WhizzEngine/Core/Engine.h"
#include "WhizzEngine/Core/Log.h"

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "");

	WhizzEngine::Log::Init();

	WhizzEngine::Engine::Init();
	WhizzEngine::Engine::Run();
	WhizzEngine::Engine::CleanUp();
}