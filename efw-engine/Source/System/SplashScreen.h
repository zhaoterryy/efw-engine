#pragma once
#include <SFML/Graphics.hpp>
// #include "ResourceManager.h"

class SplashScreen
{
public:
	SplashScreen();
	void Show(sf::RenderWindow& renderWindow);

private:
// 	ResourceManager<sf::Font> fontHolder;
	sf::Font titleFont;
	sf::Texture sfmlImage;
	sf::Texture solImage;
};