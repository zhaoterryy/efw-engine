#include "Application.h"
#include "efw-engine/EngineTypes.h"
#include "GameFramework/Scene.h"
#include "ResourceManager.h"
#include "SplashScreen.h"
#include "Renderer.h"
#include "LuaConfig.h"

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

GEngine *GEngine::instance;

void GEngine::StartGameLoop()
{
	GetCurrentScene().textureResources.AddResource("sol", "images/sol.png");
	GetCurrentScene().soundResources.AddResource("ball_hit", "sfx/golfball.wav");

	using namespace std::chrono;

	auto previous = high_resolution_clock::now();
	auto ssElapsed = duration_values<duration<long, std::nano>>::zero();

	while (ssElapsed < seconds(1) && gameState == EGameState::INITIALIZED)
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

void GEngine::Initialize()
{
	InitRenderer();
	LuaConfig::InitState(lua);
	sceneStack.push(LuaConfig::GetScene(lua));
	LuaConfig::BindCallbacks(lua, "main");
	CheckMinimumReq();
	gameState = EGameState::INITIALIZED;
}

void GEngine::InitRenderer()
{
	SplashScreen splashScreen;

	// renderWindow.create(sf::VideoMode(1920, 1080, 32), "efw-engine"); // for retina display
	renderWindow.create(sf::VideoMode(1024, 768, 32), "efw-engine");
	
	splashScreen.Show(renderWindow);
	gameState = EGameState::SPLASH_SCREEN;

	renderer = new Renderer();
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