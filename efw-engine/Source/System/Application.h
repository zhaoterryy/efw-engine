#pragma once
#include "SFML/Graphics.hpp"
#include "SplashScreen.h"
#include "sol.hpp"

class Scene;

class GEngine
{
public:
	enum class EGameState
	{
		UNINITIALIZED,
		SPLASH_SCREEN,
		INITIALIZED,
		PAUSED,
		MENU_SCREEN,
		PLAYING,
		EXITING
	};

	static GEngine& Get()
	{
		if (instance == nullptr)
			instance = new GEngine();

		return *instance;
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

	void InitLua();

	bool IsExiting();
	void GameLoop(float deltaTime);

private:
	// static instance
	static GEngine* instance;

	EGameState gameState;
	sf::RenderWindow renderWindow;
	sol::state lua;
	SplashScreen splashScreen;
	Scene* currentScene;
};
