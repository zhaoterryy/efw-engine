#include "Application.h"

GEngine* GEngine::Instance;

void GEngine::StartGameLoop()
{
	if (GameState != EGameState::INITIALIZED)
	{
		return;
	}

	RenderWindow.create(sf::VideoMode(1024, 768, 32), "efw-engine");
	GameState = EGameState::SPLASH_SCREEN;
	SplashScreen.Show(RenderWindow);

	while (!IsExiting())
	{
		GameLoop();
	}

	RenderWindow.close();
}

void GEngine::Initialize()
{
	GameState = EGameState::INITIALIZED;
}

GEngine::EGameState GEngine::GetGameState()
{
	return GameState;
}

bool GEngine::IsExiting()
{
	return false;
}

void GEngine::GameLoop()
{
}
