#pragma once

class Scene;

namespace sf 
{
	class RenderWindow;
}

class Renderer 
{
public:
	Renderer() = default;
	void Draw(sf::RenderWindow& rw, Scene& scene);

};