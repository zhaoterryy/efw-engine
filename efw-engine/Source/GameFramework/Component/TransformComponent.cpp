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
		worldTransform = host->GetParent()->GetComponent<TransformComponent>()->GetWorldTransform() + relativeTransform;
	}
	else {
		worldTransform = relativeTransform;
	}
}

void TransformComponent::SetRelativeTransform(const FTransform transform)
{
	relativeTransform = transform;
}

void TransformComponent::SetRelPosition(FVector pos)
{
	relativeTransform.position = pos;
}

void TransformComponent::SetRelRotation(float rot)
{
	relativeTransform.rotation = rot;
}

void TransformComponent::SetRelScale(FVector scl)
{
	relativeTransform.scale = scl;
}

FTransform TransformComponent::GetRelativeTransform() const
{
	return relativeTransform;
}

FTransform TransformComponent::GetWorldTransform() const
{
	return worldTransform;
}