#pragma once

#include <vector>

class Object;

class Scene
{

public:
	Scene();

	virtual void Tick(float deltaTime);
	
public:
	void AddObject(Object* obj);

protected:
	std::vector<Object*> sceneElements;
};