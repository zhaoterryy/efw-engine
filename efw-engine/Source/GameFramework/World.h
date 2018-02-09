#pragma once

#include <vector>

class World
{

public:
	World();

	virtual void Tick(float DeltaTime);
	
public:
	void AddObject(class Object* Obj);

protected:
	std::vector<Object*> SceneElements;

};