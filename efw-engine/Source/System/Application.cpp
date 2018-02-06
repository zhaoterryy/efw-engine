#include "Application.h"

Engine::EGameState Engine::GameState;
sf::RenderWindow Engine::RenderWindow;

void Engine::StartGameLoop()
{
	if (GameState != INITIALIZED)
		return;

	RenderWindow.create(sf::VideoMode(1024, 768, 32), "efw-engine");
	GameState = PLAYING;

	while (!IsExiting())
	{
		GameLoop();
	}

	RenderWindow.close();
}

void Engine::Initialize()
{
	//RenderWindow = new sf::RenderWindow();
	GameState = INITIALIZED;
}

bool Engine::IsExiting()
{
	return false;
}

void Engine::GameLoop()
{
}
