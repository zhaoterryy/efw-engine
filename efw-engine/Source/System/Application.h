#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "sol.hpp"
#include <memory>
#include <stack>
#include <functional>

class Scene;
class Renderer;

namespace sf
{
	class RenderWindow;
}

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
	void Initialize(const char* firstScene);

	EGameState GetGameState() { return gameState; }

private:
	// Only allow internal ctor & dtor
	GEngine();
	~GEngine();

	// Do not allow copying or assignment
	GEngine(const GEngine&);
	GEngine& operator=(const GEngine&);

	void InitLua();
	void InitRenderer();

	bool IsExiting();
	void GameLoop(float deltaTime);
	void CheckMinimumReq();

	std::unique_ptr<Scene> GetSceneFromLua(const char* sceneName);

private:
	// static instance
	static GEngine* instance;

	EGameState gameState;
	std::string gameTitle;
	sol::state lua;
	sf::RenderWindow renderWindow;
	std::stack<std::unique_ptr<Scene>> sceneStack;
	std::unique_ptr<Renderer> renderer;

	bool exitPressed;

	Scene& GetCurrentScene() { return *sceneStack.top(); }
};
