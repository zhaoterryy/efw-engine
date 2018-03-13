#include "Scene.h"
#include "Component/TransformComponent.h"
#include "SceneObject.h"

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

void Scene::AddObject(Object* obj)
{
	sceneElements.push_back(obj);
}

void Scene::TestPrintObjectTransforms()
{
	for (Object* const objItr : sceneElements)
	{
		std::cout << objItr->GetName() << std::endl;
		std::cout << reinterpret_cast<SceneObject*>(objItr)->GetComponent<TransformComponent>()->GetWorldTransform();
	}
}

void Scene::SetSceneName(std::string inName)
{
	name = inName;
}
