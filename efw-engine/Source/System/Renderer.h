#pragma once

class Scene;

namespace sf 
{
	class RenderWindow;
}

class Renderer 
{
public:
	Renderer(sf::RenderWindow& rw);
	void Draw(Scene& scene);

private:
	sf::RenderWindow& renderWindow;

};