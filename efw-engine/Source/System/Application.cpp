#include "Application.h"
#include "efw-engine/EngineTypes.h"
#include "GameFramework/Component/TransformComponent.h"
#include "GameFramework/Scene.h"
#include "GameFramework/SceneObject.h"

#include "SFML/Window/Window.hpp"
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
		MEMORYSTATUSEX status = { sizeof status };
		GlobalMemoryStatusEx(&status);
		return status.ullAvailPhys / (1024 * 1024 * 1024);
	}

	DWORDLONG ReadAvailableVirtualMemory()
	{
		MEMORYSTATUSEX status = { sizeof status };
		GlobalMemoryStatusEx(&status);
		return status.ullAvailVirtual / (1024 * 1024 * 1024);
	}
#endif
}

GEngine* GEngine::instance;

void GEngine::StartGameLoop()
{
	if (gameState != EGameState::INITIALIZED)
	{
		std::cerr << "StartGameLoop() called before engine was ready.\n";
		std::exit(EXIT_FAILURE);
	}

	renderWindow.create(sf::VideoMode(1024, 768, 32), "efw-engine");
	gameState = EGameState::SPLASH_SCREEN;
	splashScreen.Show(renderWindow);

	SceneObject* obj = new SceneObject();

	GetCurrentScene().AddObject(obj);
	obj->AddComponent<TransformComponent>(FVector(5), 5.0f, FVector(2));
	obj->AddComponent<TransformComponent>();
	obj->SetName("poop");

	GetCurrentScene().Tick(0);
	GetCurrentScene().TestPrintObjectTransforms();
	std::cout << "Scene: " << GetCurrentScene().GetSceneName();

	using namespace std::chrono;

	auto Previous = high_resolution_clock::now();
	auto Delay = duration_values<duration<long, std::nano>>::zero();

	while (!IsExiting())
	{
		auto Current = high_resolution_clock::now();
		auto Elapsed = Current - Previous;
		Previous = Current;
		Delay += Elapsed;

		GameLoop((float)Delay.count() / 1000000000);
		Delay = duration_values<duration<long, std::nano>>::zero();
	}
}

void GEngine::Initialize(const char* firstScene)
{
	InitLua();
	sceneStack.push(GetSceneFromLua(firstScene));
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

std::unique_ptr<Scene> GEngine::GetSceneFromLua(const char* sceneName)
{
	// grab all scenes
	sol::table scenes = lua["scenes"];

	// check if sceneName exists
	sol::optional<sol::table> sceneTbl = scenes[sceneName];
	if (sceneTbl != sol::nullopt)
	{
		Scene* newScene = new Scene();

		newScene->SetSceneName(sceneTbl->get_or<std::string>("name", "Untitled"));

		// get entities and iterate through if found
		sol::optional<sol::table> entitiesTbl = sceneTbl->get<sol::table>("entities");
		if (entitiesTbl != sol::nullopt)
		{
			entitiesTbl->for_each([&newScene = newScene](auto k, auto v)
			{
				SceneObject* newEntity = new SceneObject();

				// get current entity's properties
				sol::table entityTbl = v.template as<sol::table>();

				// get entity name if available
				newEntity->SetName(entityTbl.get_or<std::string>("name", "Untitled"));

				// get components and iterate through if found
				sol::optional<sol::table> componentsTbl = entityTbl.get<sol::table>("components");
				if (componentsTbl != sol::nullopt)
				{
					componentsTbl->for_each([newEntity](auto comp_k, auto comp_v)
					{
						// check for Transform
						if (comp_k.template as<std::string>() == "transform")
						{
							if (comp_v.template is<FTransform>())
							{
								newEntity->AddComponent<TransformComponent>(comp_v.template as<FTransform>());
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
										newEntity->AddComponent<TransformComponent>(*p, *r, *s);
									}
								}
								else
								{
									std::cerr << "Error parsing transform component from lua." << std::endl;
									std::exit(EXIT_FAILURE);
								}
							}
						}
					});
				}
				newScene->AddObject(newEntity);
			});
		}
		return std::unique_ptr<Scene>(newScene);
	}
	return std::unique_ptr<Scene>(nullptr);
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
	{
		// utype FTransform
		sol::constructors<FTransform(), void(), void(FVector, float, FVector)> ctor;
		sol::usertype<FTransform> utype(ctor,
			"Position", &FTransform::Position,
			"Rotation", &FTransform::Rotation,
			"Scale", &FTransform::Scale
		);
		lua.set_usertype("FTransform", utype);
	}

	try
	{
		lua.script_file("Scripts/main.lua");
		std::cout << "hello!" << std::endl;
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

void GEngine::GameLoop(float deltaTime)
{
	GetCurrentScene().Tick(deltaTime);
	// 	std::cout << std::fixed << deltaTime << std::endl;
}
