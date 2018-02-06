#pragma once
#include "SFML/Graphics.hpp"
#include "SplashScreen.h"

class GEngine
{
public:
	enum class EGameState
	{
		UNINITIALIZED,
		INITIALIZED,
		SPLASH_SCREEN,
		PAUSED,
		MENU_SCREEN,
		PLAYING,
		EXITING
	};

	static GEngine& Get()
	{
		if (Instance == nullptr)
			Instance = new GEngine();

		return *Instance;
	}
	void StartGameLoop();
	void Initialize();

	EGameState GetGameState();

private:
	// Only allow internal ctor & dtor
	GEngine() = default;
	~GEngine() = default;

	// Do not allow copying or assignment
	GEngine(const GEngine&);
	GEngine& operator=(const GEngine&);

	// static instance
	static GEngine* Instance;

	bool IsExiting();
	void GameLoop();

	EGameState GameState;
	sf::RenderWindow RenderWindow;
	SplashScreen SplashScreen;
};