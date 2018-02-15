#include "Application.h"

#include "efw-engine/EngineTypes.h"
#include <iostream>
#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#endif

namespace 
{
#ifdef _WIN32
	bool IsOnlyInstance(const LPCTSTR GameTitle)
	{
		HANDLE handle = CreateMutex(NULL, TRUE, GameTitle);

		if (GetLastError() != ERROR_SUCCESS)
		{
			HWND hWnd = FindWindow(GameTitle, NULL);
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

	bool CheckStorage(const DWORDLONG DiskSpaceNeeded)
	{
		int const Drive = _getdrive();
		struct _diskfree_t DiskFree;

		_getdiskfree(drive, &DiskFree);
		
		unsigned _int64 const NeededClusters = DiskSpaceNeeded / (DiskFree.sectors_per_cluster * DiskFree.bytes_per_sector);

		if (DiskFree.avail_clusters < NeededClusters)
		{
			std::cerr << "CheckStorage Error: You do not have enough disk space.";
			return false;
		}
		return true;
	}

	bool CheckMemory(const DWORDLONG PhysicalRamNeeded, const DWORDLONG VirtualRamNeeded)
	{
		MEMORYSTATUSEX Status;
		GlobalMemoryStatusEx(&Status);
		if (Status.ullTotalPhys < PhysicalRamNeeded)
		{
			std::cerr << "CheckMemory Error: You do not have enough physical memory.";
			return false;
		}

		if (Status.ullAvailVirtual < VirtualRamNeeded)
		{
			std::cerr << "CheckMemory Error: You do not have enough virtual memory.";
			return false;
		}

		char *buff = new char[VirtualRamNeeded];
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
		DWORD BufSize = sizeof(DWORD);
		DWORD DwMHz = 0;
		DWORD Type = REG_DWORD;
		HKEY HKey;

		long Error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);

		if (Error == ERROR_SUCCESS)
		{
			RegQueryValueEx(HKey, "~MHz", NULL, &type, (LPBYTE)&DwMHz, &BufSize);
		}
		return DwMHz;
	}
	
	std::string ReadCPUIdentifier()
	{
		DWORD BufSize = sizeof(TCHAR) * 1024;
		TCHAR StrId[1024];
		DWORD Type = REG_SZ;
		HKEY HKey;

		long Error = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey);

		if (Error == ERROR_SUCCESS)
		{
			RegQueryValueEx(hKey, "ProcessorNameString", NULL, &type, (LPBYTE)&strId, &BufSize);
		}
		return std::string(strId);
	}

	DWORDLONG ReadAvailableRAM()
	{
		MEMORYSTATUSEX Status;
		Status.dwLength = sizeof(Status);
		GlobalMemoryStatusEx(&status);
		return status.ullAvailPhys / (1024 * 1024 * 1024);
	}

	DWORDLONG ReadAvailableVirtualMemory()
	{
		MEMORYSTATUSEX Status;
		Status.dwLength = sizeof(Status);
		GlobalMemoryStatusEx(&Status);
		return Status.ullAvailVirtual / (1024 * 1024 * 1024);
	}
#endif
}

GEngine* GEngine::Instance;

void GEngine::StartGameLoop()
{
	if (GameState != EGameState::INITIALIZED)
	{
		std::cerr << "StartGameLoop() called before engine was ready.\n";
		std::exit(EXIT_FAILURE);
	}

	if (MsPerTick == 0)
	{
		std::cerr << "StartGameLoop() called before GEngine::MsPerTick was initialized.\n";
		std::exit(EXIT_FAILURE);
	}

 	RenderWindow.create(sf::VideoMode(1024, 768, 32), "efw-engine");
	GameState = EGameState::SPLASH_SCREEN;
 	SplashScreen.Show(RenderWindow);

	using namespace std::chrono;

	auto Previous = high_resolution_clock::now();
	auto dMsPerTick = nanoseconds(milliseconds(MsPerTick));
	auto Delay = duration_values<duration<long, std::nano>>::zero();

	testScene = new World();
	testObj1 = new Object(FVector(7, 7), 0, FVector());
	testObj2 = new Object(FVector(3, 3), 0, FVector());
	testScene->AddObject(testObj1);
	testScene->AddObject(testObj2);
	testScene->Tick(0);
	
	std::cout << "\nObj1 WorPos : " << testObj1->GetWorldTransform().Position;
	std::cout << "\nObj2 WorPos : " << testObj2->GetWorldTransform().Position;

	testObj1->AddChild(testObj2);
	testScene->Tick(0);

	std::cout << "\nObj1 WorPos : " << testObj1->GetWorldTransform().Position;
	std::cout << "\nObj2 WorPos : " << testObj2->GetWorldTransform().Position;

	while (!IsExiting())
	{
		auto Current = high_resolution_clock::now();
		auto Elapsed = Current - Previous;
		Previous = Current;
		Delay += Elapsed;

		if (!IsExiting() && Delay >= dMsPerTick)
		{
			DeltaTime = ((float)Delay.count() / 1000000000);
			Delay -= dMsPerTick;
			GameLoop();
		}
	}

	//RenderWindow.close();
}

void GEngine::Initialize()
{
	InitLua();
	MsPerTick = 16;
	GameState = EGameState::INITIALIZED;
}

GEngine::EGameState GEngine::GetGameState()
{
	return GameState;
}

void GEngine::SetMsPerTick(int InMsPerTick)
{
	MsPerTick = InMsPerTick;
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

	// get "worlds" from lua
	sol::table worldsTable_s = lua["worlds"];
	std::vector<sol::table> worldsTable;

	worldsTable_s.for_each([&worldsTable, worldsTable_s](auto key, auto value)
	{
		worldsTable.push_back(worldsTable_s.get<sol::table>(key));
	});

	for (const auto& wt : worldsTable)
	{
		std::string worldName = wt.get<std::string>("name");

		// get entities table
		sol::table entitiesTable = wt["entities"];

		for (const auto& entity : entitiesTable)
		{
			// get current entity table
			sol::table et = entitiesTable.get<sol::table>(entity.first);
			// check if name is valid
			sol::optional<std::string> name = et["name"];
			if (name != sol::nullopt)
			{
				std::cout << "Entity: " << name.value() << std::endl;
			}

			// check if there are any components
			sol::optional<sol::table> componentsTable = et["components"];
			if (componentsTable != sol::nullopt)
			{
				sol::optional<sol::table> actorCompTable = componentsTable.value()["actorComponent"];
				if (actorCompTable != sol::nullopt)
				{
					// create ActorComponent here
					FVector v = actorCompTable->get<FVector>("Location");
					std::cout << "Test vector X: " << v.X << std::endl;
					std::cout << "Test vector Y: " << v.Y << std::endl;
				}

			}
		}
	}
}

bool GEngine::IsExiting()
{
	return false;
}

void GEngine::GameLoop()
{
}
