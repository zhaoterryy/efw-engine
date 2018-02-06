#include "Application.h"

#include "efw-engine/EngineTypes.h"
#include <iostream>
#include <chrono>

GEngine* GEngine::Instance;

void GEngine::StartGameLoop()
{
	if (GameState != EGameState::INITIALIZED)
	{
		std::cerr << "StartGameLoop() called before engine was ready.\n";
		std::exit(EXIT_FAILURE);
	}

	if (MsPerTick == 0)
	{
		std::cerr << "StartGameLoop() called before GEngine::MsPerTick was initialized.\n";
		std::exit(EXIT_FAILURE);
	}

 	RenderWindow.create(sf::VideoMode(1024, 768, 32), "efw-engine");
	GameState = EGameState::SPLASH_SCREEN;
 	SplashScreen.Show(RenderWindow);

	using namespace std::chrono;

	auto Previous = high_resolution_clock::now();
	auto dMsPerTick = nanoseconds(milliseconds(MsPerTick));
	auto Delay = duration_values<duration<long, std::nano>>::zero();

	while (!IsExiting())
	{
		auto Current = high_resolution_clock::now();
		auto Elapsed = Current - Previous;
		Previous = Current;
		Delay += Elapsed;

		while (!IsExiting() && Delay >= dMsPerTick)
		{
			long long DeltaTime = (Delay.count() / 1000000);
			std::cout << DeltaTime << std::endl;
			Delay -= dMsPerTick;
			GameLoop();
		}
	}

	RenderWindow.close();
}

void GEngine::Initialize()
{
	InitLua();
	MsPerTick = 16;
	GameState = EGameState::INITIALIZED;
}

GEngine::EGameState GEngine::GetGameState()
{
	return GameState;
}

void GEngine::SetMsPerTick(int InMsPerTick)
{
	MsPerTick = InMsPerTick;
}

void GEngine::InitLua()
{
	lua.open_libraries();

	{
		// usertype FVector
		sol::constructors<FVector(), void(), void(float, float)> ctor;
		sol::usertype<FVector> utype(ctor,
			"x", &FVector::X,
			"y", &FVector::Y
		);
		lua.set_usertype("FVector", utype);
	}

	try
	{
		lua.script_file("Scripts/test.lua");
	}
	catch (const sol::error& err)
	{
		std::cerr << err.what();
		std::exit(EXIT_FAILURE);
	}
}

bool GEngine::IsExiting()
{
	return false;
}

void GEngine::GameLoop()
{
}
