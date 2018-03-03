#include "Application.h"
#include "efw-engine/EngineTypes.h"
#include "GameFramework/Component/TransformComponent.h"
#include "GameFramework/Scene.h"
#include "GameFramework/SceneObject.h"

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

	currentScene->AddObject(obj);
	obj->AddComponent<TransformComponent>(FVector(5), 5, FVector(2));
	obj->SetName("poop");

	currentScene->Tick(0);
	currentScene->TestPrintObjectTransforms();
	
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

	//RenderWindow.close();
}

void GEngine::Initialize()
{
	InitScene();
	InitLua();
	CheckMinimumReq();
	gameState = EGameState::INITIALIZED;
}

void GEngine::InitScene()
{
	currentScene = new Scene();
}

void GEngine::CheckMinimumReq()
{
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
		lua.script_file("Scripts/test.lua");
		std::cout << "hello!" << std::endl;
	}
	catch (const sol::error& err)
	{
		std::cerr << err.what();
		std::exit(EXIT_FAILURE);
	}

	// get "scenes" from lua
	sol::table scenesTable_s = lua["scenes"];
	std::vector<sol::table> scenesTable;

	scenesTable_s.for_each([&scenesTable, scenesTable_s](auto key, auto value)
	{
		scenesTable.push_back(scenesTable_s.get<sol::table>(key));
	});

	for (const auto& wt : scenesTable)
	{
		std::string sceneName = wt.get<std::string>("name");

		// get entities table
		sol::table entitiesTable = wt["entities"];

		for (const auto& entity : entitiesTable)
		{
			SceneObject* newObject = new SceneObject();
			// get current entity table
			sol::table et = entitiesTable.get<sol::table>(entity.first);
			// check if name is valid
			sol::optional<std::string> name = et["name"];
			if (name != sol::nullopt)
			{
				newObject->SetName(name.value());
			}

			// check if there are any components
			sol::optional<sol::table> componentsTable = et["components"];
			if (componentsTable != sol::nullopt)
			{
				componentsTable.value().for_each([newObject](auto key, auto value)
				{
					if (key.as<std::string>() == "transform")
					{
						if (value.is<FTransform>())
						{
							TransformComponent* transComp = new TransformComponent(newObject, value.as<FTransform>());
						}
						else
						{
							sol::optional<sol::table> transTable = value.as<sol::table>();

							if (transTable != sol::nullopt)
							{
								FVector p = transTable->get<FVector>("Position");
								float r = transTable->get<float>("Rotation");
								FVector s = transTable->get<FVector>("Scale");
								TransformComponent* transComp = new TransformComponent(newObject, p, r, s);
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
			currentScene->AddObject(newObject);
		}
	}
}

bool GEngine::IsExiting()
{
	return false;
}

void GEngine::GameLoop(float deltaTime)
{
	currentScene->Tick(deltaTime);
// 	std::cout << std::fixed << deltaTime << std::endl;
}
