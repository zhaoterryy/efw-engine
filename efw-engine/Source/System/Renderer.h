#pragma once

class Scene;

class Renderer 
{
public:
    Renderer() = default;
	void Draw(Scene& scene);
};