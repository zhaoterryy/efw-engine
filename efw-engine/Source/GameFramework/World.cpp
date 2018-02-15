#include "World.h"
#include "Object.h"

World::World()
{

}

void World::Tick(float DeltaTime)
{
	for (Object* obj : SceneElements)
	{
		obj->Tick(DeltaTime);
	}
}

void World::AddObject(Object* Obj)
{
	SceneElements.push_back(Obj);
}
