#include "Application.h"

#include "efw-engine/EngineTypes.h"
#include <iostream>
#include <chrono>
#include "GameFramework/Component/TransformComponent.h"

GEngine* GEngine::Instance;

void GEngine::StartGameLoop()
{
	if (GameState != EGameState::INITIALIZED)
	{
		std::cerr << "StartGameLoop() called before engine was ready.\n";
		std::exit(EXIT_FAILURE);
	}

 	//RenderWindow.create(sf::VideoMode(1024, 768, 32), "efw-engine");
	GameState = EGameState::SPLASH_SCREEN;
 	//SplashScreen.Show(RenderWindow);

	/*testScene = new World();
	testObj1 = new Object(FVector3(7, 7, 0), FRotator(), FVector3());
	testObj2 = new Object(FVector3(3, 3, 0), FRotator(), FVector3());
	testScene->AddObject(testObj1);
	testScene->AddObject(testObj2);
	testScene->Tick(0);

	std::cout << "\nObj1 WorTrans : ";
	testObj1->GetWorldTransform().PrintPosition();
	std::cout << "\nObj2 WorTrans : ";
	testObj2->GetWorldTransform().PrintPosition();

	testObj1->AddChild(testObj2);
	testScene->Tick(0);

	std::cout << "\nObj1 WorTrans : ";
	testObj1->GetWorldTransform().PrintPosition();
	std::cout << "\nObj2 WorTrans : ";
	testObj2->GetWorldTransform().PrintPosition();*/

	// Object* testObj1 = new Object();
	// TransformComponent* test = testObj1->GetComponent<TransformComponent>();
	// std::cout << (test == nullptr);
	//testObj1->GetComponent<TransformComponent>()->GetWorldTransform().PrintPosition();
	//testObj1

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
	InitLua();
	GameState = EGameState::INITIALIZED;
}

GEngine::EGameState GEngine::GetGameState()
{
	return GameState;
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

void GEngine::GameLoop(float DeltaTime)
{
	std::cout << std::fixed << DeltaTime << std::endl;
}
