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

	static sol::state& GLua()
	{
		return Get().lua;
	}

	void StartGameLoop();
	void Initialize();
	void InitScene();

	inline EGameState GetGameState() { return gameState; }


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
	void CheckMinimumReq();

private:
	// static instance
	static GEngine* instance;

	EGameState gameState;
	std::string gameTitle;
	sol::state lua;
	sf::RenderWindow renderWindow;
	SplashScreen splashScreen;
	Scene* currentScene;
};
