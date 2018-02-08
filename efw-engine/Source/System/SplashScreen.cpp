#include "SplashScreen.h"
#include "Application.h"

void SplashScreen::Show(sf::RenderWindow& RenderWindow)
{
	sf::Texture Image;

	if (Image.loadFromFile("Images/SplashScreen.png") != true)
		return;

	sf::Sprite Sprite(Image);

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
