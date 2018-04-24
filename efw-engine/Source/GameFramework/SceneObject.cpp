#include "SceneObject.h"
#include "Component/AudioComponent.h"
#include <SFML/Window/Keyboard.hpp>

void SceneObject::Tick(float deltaTime)
{
	Object::Tick(deltaTime);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if (GetComponent<AudioComponent>() != nullptr)
			GetComponent<AudioComponent>()->PlaySound("ball_hit");
	}

	for (auto& comp : components)
		comp->Tick(deltaTime);

	for (SceneObject* child : children)
		child->Tick(deltaTime);

}

FTransform SceneObject::GetRelativeTransform()
{
	return GetComponent<TransformComponent>()->GetRelativeTransform();
}

void SceneObject::SetRelativeTransform(const FTransform transform)
{
	GetComponent<TransformComponent>()->SetRelativeTransform(transform);
}

FTransform SceneObject::GetWorldTransform()
{
	return GetComponent<TransformComponent>()->GetWorldTransform();
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