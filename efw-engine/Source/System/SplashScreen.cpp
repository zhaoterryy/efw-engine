#include "SplashScreen.h"
#include "Application.h"

SplashScreen::SplashScreen()
{
	titleFont.loadFromFile("fonts/courbd.ttf");
	sfmlImage.loadFromFile("images/sfml-logo-small.png");
	solImage.loadFromFile("images/sol.png");
}

void SplashScreen::Show(sf::RenderWindow& renderWindow)
{
	sf::Text ssTitle("eco frag warriors\n\n\n\npowered by: ", titleFont);
	ssTitle.setPosition(50, renderWindow.getSize().y / 2 - ssTitle.getGlobalBounds().height / 2);

	sf::Sprite sfmlSprite(sfmlImage);
	sfmlSprite.setScale(0.5f, 0.5f);
	sfmlSprite.setPosition((renderWindow.getSize().x / 3) * 2 - 50, renderWindow.getSize().y / 2 + 50);

	sf::Sprite solSprite(solImage);
	solSprite.setScale(0.5f, 0.5f);
	solSprite.setPosition(renderWindow.getSize().x / 3, renderWindow.getSize().y / 2);

	renderWindow.draw(sfmlSprite);
	renderWindow.draw(solSprite);
	renderWindow.draw(ssTitle);
	renderWindow.display();
}