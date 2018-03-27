#include "TransformComponent.h"
#include "GameFramework/SceneObject.h"

TransformComponent::TransformComponent(SceneObject* inHost) :
	BaseComponent(inHost),
	relativeTransform(FTransform())
{
}

TransformComponent::TransformComponent(SceneObject* inHost, FTransform trans) :
	BaseComponent(inHost),
	relativeTransform(trans)
{
}

TransformComponent::TransformComponent(SceneObject* inHost, FVector pos, float rot, FVector scl) :
	BaseComponent(inHost)
{
	relativeTransform.position = pos;
	relativeTransform.rotation = rot;
	relativeTransform.scale = scl;
}

void TransformComponent::Tick(float deltaTime)
{
	if (host->GetParent() != nullptr) {
		//WorldTransform = Host->GetParent()->GetComponent<TransformComponent>()->GetWorldTransform() + RelativeTransform;
	}
	else {
		worldTransform = relativeTransform;
	}
}

void TransformComponent::SetRelativeTransform(const FTransform transform)
{
	relativeTransform = transform;
}

FTransform TransformComponent::GetRelativeTransform() const
{
	return relativeTransform;
}

FTransform TransformComponent::GetWorldTransform() const
{
	return worldTransform;
}