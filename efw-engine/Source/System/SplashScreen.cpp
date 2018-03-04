#include "SplashScreen.h"
#include "Application.h"

void SplashScreen::Show(sf::RenderWindow& RenderWindow)
{
	sf::Texture image;

	sol::table splashTable = GEngine::GLua()["splash_screen"];
	std::string imagePath = splashTable.get<std::string>("img_path");

	if (image.loadFromFile(imagePath) != true)
		return;

	sf::Sprite Sprite(image);

	RenderWindow.draw(Sprite);
	RenderWindow.display();

	sf::Event Event;

	while (RenderWindow.pollEvent(Event))
	{
		if (Event.type == sf::Event::KeyPressed
			|| Event.type == sf::Event::MouseButtonPressed
			|| Event.type == sf::Event::Closed)
		{
			return;
		}
	}
}
