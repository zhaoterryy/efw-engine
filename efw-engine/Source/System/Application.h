#pragma once
#include "SFML/Graphics.hpp"

class Engine
{
public:
	static void StartGameLoop();
	static void Initialize();

private:
	enum EGameState
	{
		UNINITIALIZED,
		INITIALIZED,
		SPLASH_SCREEN,
		PAUSED,
		MENU_SCREEN,
		PLAYING,
		EXITING
	};

	static bool IsExiting();
	static void GameLoop();

	static EGameState GameState;
	static sf::RenderWindow RenderWindow;
};