#include "Scene.h"
#include "Object.h"

Scene::Scene()
{

}

void Scene::Tick(float deltaTime)
{
	for (Object* obj : sceneElements)
	{
		obj->Tick(deltaTime);
	}
}

void Scene::AddObject(class Object* obj)
{
	sceneElements.push_back(obj);
}
