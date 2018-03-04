#include <SFML/Graphics.hpp>
#include "System/Application.h"

int main()
{
	GEngine::Get().Initialize("main");
	GEngine::Get().StartGameLoop();

	return 0;
}