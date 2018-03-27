#include "Application.h"
#include "efw-engine/EngineTypes.h"
#include "GameFramework/Component/TransformComponent.h"
#include "GameFramework/Component/RenderComponent.h"
#include "GameFramework/Component/LuaComponent.h"
#include "GameFramework/Component/AudioComponent.h"
#include "GameFramework/Scene.h"
#include "GameFramework/SceneObject.h"
#include "ResourceManager.h"
#include "SplashScreen.h"
#include "Renderer.h"

#include <SFML/Window/Window.hpp>
#include <iostream>
#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#endif

namespace
{
#ifdef _WIN32
	bool IsOnlyInstance(const LPCTSTR title)
	{
		HANDLE handle = CreateMutex(NULL, TRUE, title);

		if (GetLastError() != ERROR_SUCCESS)
		{
			HWND hWnd = FindWindow(title, NULL);
			if (hWnd)
			{
				ShowWindow(hWnd, SW_SHOWNORMAL);
				SetFocus(hWnd);
				SetForegroundWindow(hWnd);
				SetActiveWindow(hWnd);
				return false;
			}
		}
		return true;
	}

	bool IsEnoughDiskSpace(const DWORDLONG diskSpaceNeeded)
	{
		int const drive = _getdrive();
		struct _diskfree_t DiskFree;

		_getdiskfree(drive, &DiskFree);

		unsigned _int64 const neededClusters = diskSpaceNeeded / (DiskFree.sectors_per_cluster * DiskFree.bytes_per_sector);

		if (DiskFree.avail_clusters < neededClusters)
		{
			std::cerr << "CheckStorage Error: You do not have enough disk space.";
			return false;
		}
		return true;
	}

	bool CheckMemory(const DWORDLONG physicalRamNeeded, const DWORDLONG virtualRamNeeded)
	{
		MEMORYSTATUSEX status;
		GlobalMemoryStatusEx(&status);
		if (status.ullTotalPhys < physicalRamNeeded)
		{
			std::cerr << "CheckMemory Error: You do not have enough physical memory.";
			return false;
		}

		if (status.ullAvailVirtual < virtualRamNeeded)
		{
			std::cerr << "CheckMemory Error: You do not have enough virtual memory.";
			return false;
		}

		char *buff = new char[virtualRamNeeded];
		if (buff)
		{
			delete[] buff;
		}
		else
		{
			std::cerr << "CheckMemory Error: Not enough contiguous memory.";
			return false;
		}
		return true;
	}

	DWORD ReadCPUSpeed()
	{
		DWORD bufSize = sizeof(DWORD);
		DWORD dwMHz = 0;
		DWORD type = REG_DWORD;
		HKEY hKey;

		long error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);

		if (error == ERROR_SUCCESS)
		{
			RegQueryValueEx(hKey, "~MHz", NULL, &type, (LPBYTE)&dwMHz, &bufSize);
		}
		return dwMHz;
	}

	std::string ReadCPUIdentifier()
	{
		DWORD bufSize = sizeof(TCHAR) * 1024;
		TCHAR strId[1024];
		DWORD type = REG_SZ;
		HKEY hKey;

		long error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);

		if (error == ERROR_SUCCESS)
		{
			RegQueryValueEx(hKey, "ProcessorNameString", NULL, &type, (LPBYTE)&strId, &bufSize);
		}
		return std::string(strId);
	}

	DWORDLONG ReadAvailableRAM()
	{
		MEMORYSTATUSEX status = {sizeof status};
		GlobalMemoryStatusEx(&status);
		return status.ullAvailPhys / (1024 * 1024 * 1024);
	}

	DWORDLONG ReadAvailableVirtualMemory()
	{
		MEMORYSTATUSEX status = {sizeof status};
		GlobalMemoryStatusEx(&status);
		return status.ullAvailVirtual / (1024 * 1024 * 1024);
	}
#endif
}

GEngine *GEngine::instance;
GEngine::GEngine() = default;
GEngine::~GEngine() = default;

void GEngine::StartGameLoop()
{
	if (gameState != EGameState::INITIALIZED)
	{
		std::cerr << "StartGameLoop() called before engine was ready.\n";
		std::exit(EXIT_FAILURE);
	}

	SplashScreen splashScreen;
	renderWindow.create(sf::VideoMode(1024, 768, 32), "efw-engine");
	gameState = EGameState::SPLASH_SCREEN;
	splashScreen.Show(renderWindow);

	SceneObject *obj = new SceneObject();

	GetCurrentScene().AddObject(obj);
	obj->AddComponent<TransformComponent>(FVector(100), 45.0f, FVector(0.25f));
	obj->AddComponent<AudioComponent>();

	GetCurrentScene().textureResources.AddResource("sol", "images/sol.png");
	GetCurrentScene().soundResources.AddResource("ball_hit", "sfx/golfball.wav");

	RenderComponent& rc = obj->AddComponent<RenderComponent>();
	rc.spriteId = "sol";

	obj->SetName("poop");

	GetCurrentScene().Tick(0);
	std::cout << "Scene: " << GetCurrentScene().GetSceneName() << std::endl;
	GetCurrentScene().TestPrintObjectTransforms();

	using namespace std::chrono;

	auto previous = high_resolution_clock::now();
	auto ssElapsed = duration_values<duration<long, std::nano>>::zero();

	while (ssElapsed < seconds(2))
	{
		auto current = high_resolution_clock::now();
		ssElapsed += current - previous;
		previous = current;

		sf::Event event;
		if (renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				exitPressed = true;
		}
	}

	while (!IsExiting())
	{
		auto current = high_resolution_clock::now();
		GameLoop((float)(current - previous).count() / 1000000000);
		previous = current;

		sf::Event event;
		if (renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				exitPressed = true;
		}
	}

	renderWindow.close();
}

void GEngine::Initialize(const char *firstScene)
{
	InitLua();
	InitRenderer();
	sceneStack.push(GetSceneFromLua(firstScene));
	lua["test"]();
	CheckMinimumReq();
	gameState = EGameState::INITIALIZED;
}

void GEngine::CheckMinimumReq()
{
#ifdef _WIN32
	if (!IsOnlyInstance(gameTitle.c_str()))
	{
		std::cerr << "There is another instance running already.";
		std::exit(EXIT_FAILURE);
	}

	if (!IsEnoughDiskSpace(314572800))
	{
		std::cerr << "There is not enough disk space to play this game.";
		std::exit(EXIT_FAILURE);
	}

	std::cout << "RAM: " << ReadAvailableRAM() << " GB" << std::endl;
	std::cout << "CPU MHz: " << ReadCPUSpeed() << std::endl;
	std::cout << "CPU Architecture: " << ReadCPUIdentifier() << std::endl;
#endif
}

std::unique_ptr<Scene> GEngine::GetSceneFromLua(const char *sceneName)
{
	// grab all scenes
	sol::table scenes = lua["scenes"];

	// check if sceneName exists
	sol::optional<sol::table> sceneTbl = scenes[sceneName];
	if (sceneTbl != sol::nullopt)
	{
		Scene *newScene = new Scene();

		newScene->SetSceneName(sceneTbl->get_or<std::string>("name", "Untitled"));

		// get entities and iterate through if found
		sol::optional<sol::table> objectsTbl = sceneTbl->get<sol::table>("object_list");
		if (objectsTbl != sol::nullopt)
		{
			sceneTbl->set("objects", sol::new_table());
			objectsTbl->for_each([&newScene = newScene, &sceneTbl ](auto k, auto v) {
				std::string id = k.template as<std::string>();
				SceneObject *newObject = new SceneObject();

				// get current object's properties
				sol::table objectTbl = v.template as<sol::table>();

				// get object name if available
				newObject->SetName(objectTbl.get_or<std::string>("name", id));

				// get components and iterate through if found
				sol::optional<sol::table> componentsTbl = objectTbl.get<sol::table>("components");
				if (componentsTbl != sol::nullopt)
				{
					componentsTbl->for_each([newObject](auto comp_k, auto comp_v) {
						// check for Transform
						if (comp_k.template as<std::string>() == "transform")
						{
							if (comp_v.template is<FTransform>())
							{
								newObject->AddComponent<TransformComponent>(comp_v.template as<FTransform>());
							}
							else
							{
								sol::optional<sol::table> transTable = comp_v.template as<sol::table>();
								if (transTable != sol::nullopt)
								{
									sol::optional<FVector> p = transTable->get<FVector>("position");
									sol::optional<float> r = transTable->get<float>("rotation");
									sol::optional<FVector> s = transTable->get<FVector>("scale");
									if (p != sol::nullopt && r != sol::nullopt && s != sol::nullopt)
									{
										newObject->AddComponent<TransformComponent>(*p, *r, *s);
									}
								}
								else
								{
									std::cerr << "Error parsing transform component from lua." << std::endl;
									std::exit(EXIT_FAILURE);
								}
							}
						}
						if (comp_k.template as<std::string>() == "sprite_id")
						{
							newObject->AddComponent<RenderComponent>(comp_v.template as<std::string>());
						}
					});
				}
				newScene->AddObject(newObject);
				sceneTbl.value()["objects"][id] = newObject;
			});
		}
		return std::unique_ptr<Scene>(newScene);
	}
	return std::unique_ptr<Scene>(nullptr);
}

void GEngine::InitLua()
{
	lua.open_libraries();

	lua.new_usertype<FVector>("Vector",
							  sol::constructors<void(), void(float, float)>(),
							  "x", &FVector::x,
							  "y", &FVector::y);

	lua.new_usertype<FTransform>("Transform",
								 sol::constructors<void(), void(FVector, float, FVector)>(),
								 "position", &FTransform::position,
								 "rotation", &FTransform::rotation,
								 "scale", &FTransform::scale);

	lua.new_usertype<BaseComponent>("BaseComponent",
									"new", sol::no_constructor,
									"internal_tick", &BaseComponent::Tick);

	lua.new_usertype<LuaComponent>("Component",
								   sol::base_classes, sol::bases<BaseComponent>(),
								   "new", sol::no_constructor,
								   "tick", &LuaComponent::luaTick);

	lua.new_usertype<TransformComponent>("TransformComponent",
										 "new", sol::no_constructor,
										 sol::base_classes, sol::bases<BaseComponent>(),
										 "relative_transform", sol::property(&TransformComponent::SetRelativeTransform, &TransformComponent::GetRelativeTransform),
										 "world_transform", sol::property(&TransformComponent::GetWorldTransform));

	lua.new_usertype<RenderComponent>("RenderComponent",
									  "new", sol::no_constructor,
									  sol::base_classes, sol::bases<BaseComponent>(),
									  "spriteId", &RenderComponent::spriteId);

	lua.new_usertype<Object>("Object",
							 "name", sol::property(&Object::SetName, &Object::GetName),
							 "new", sol::no_constructor);

	lua.new_usertype<SceneObject>("SceneObject",
								  sol::base_classes, sol::bases<Object>(),
								  "new_component", &SceneObject::Lua_NewComponent,
								  "get_transform_comp", &SceneObject::GetComponent<TransformComponent>,
								  "parent", sol::property(&SceneObject::SetParent, &SceneObject::GetParent),
								  "add_child", &SceneObject::AddChild);

	// half assed scene utype
	lua.new_usertype<Scene>("Scene",
							"add_object", &Scene::AddObject);

	try
	{
		lua.script_file("Scripts/main.lua");
	}
	catch (const sol::error &err)
	{
		std::cerr << err.what();
		std::exit(EXIT_FAILURE);
	}
}

void GEngine::InitRenderer()
{
	renderer = std::make_unique<Renderer>();
}

bool GEngine::IsExiting()
{
	return exitPressed;
}

void GEngine::GameLoop(float deltaTime)
{
	renderer->Draw(renderWindow, GetCurrentScene());
	GetCurrentScene().Tick(deltaTime);
}