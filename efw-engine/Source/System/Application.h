#pragma once
#include "SFML/Graphics.hpp"
#include "SplashScreen.h"
#include "sol.hpp"
#include "../GameFramework/World.h"
#include "../GameFramework/Object.h"

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

	void InitLua();

	bool IsExiting();
	void GameLoop(float DeltaTime);

private:
	// static instance
	static GEngine* Instance;

	EGameState GameState;
	sf::RenderWindow RenderWindow;
	sol::state lua;
	SplashScreen SplashScreen;

	World* testScene;
	Object* testObj1;
	Object* testObj2;
};