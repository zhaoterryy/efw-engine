#include "SceneObject.h"

void SceneObject::Tick(float deltaTime)
{
	Object::Tick(deltaTime);

	for (auto& comp : components)
		comp->Tick(deltaTime);

	for (SceneObject* child : children)
		child->Tick(deltaTime);

}

void SceneObject::SetParent(SceneObject* inObj)
{
	parent = inObj;
}

void SceneObject::AddChild(SceneObject* inObj)
{
	children.push_back(inObj);
	inObj->SetParent(this);
}
