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
	relativeTransform.Position = pos;
	relativeTransform.Rotation = rot;
	relativeTransform.Scale = scl;
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
	relativeTransform.Position = pos;
}

void TransformComponent::SetRelRotation(float rot)
{
	relativeTransform.Rotation = rot;
}

void TransformComponent::SetRelScale(FVector scl)
{
	relativeTransform.Scale = scl;
}

FTransform TransformComponent::GetRelativeTransform() const
{
	return relativeTransform;
}

FTransform TransformComponent::GetWorldTransform() const
{
	return worldTransform;
}