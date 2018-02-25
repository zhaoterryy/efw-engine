#include "TransformComponent.h"

TransformComponent::TransformComponent(Object* host)
{
	TransformComponent(host, FVector3(), FRotator(), FVector3());
}

TransformComponent::TransformComponent(Object* host, FTransform trans)
{
	TransformComponent(host, trans.Position, trans.Rotation, trans.Scale);
}

TransformComponent::TransformComponent(Object* host, FVector3 pos, FRotator rot, FVector3 scl)
{
	Host = host;
	RelativeTransform.Position = pos;
	RelativeTransform.Rotation = rot;
	RelativeTransform.Scale = scl;
}

void TransformComponent::Tick(float DeltaTime)
{
	if (Host->GetParent() != nullptr) {
		//WorldTransform = Host->GetParent()->GetComponent<TransformComponent>()->GetWorldTransform() + RelativeTransform;
	}
	else {
		WorldTransform = RelativeTransform;
	}
}

void TransformComponent::SetRelativeTransform(const FTransform InTransform)
{
	RelativeTransform = InTransform;
}

FTransform TransformComponent::GetRelativeTransform() const
{
	return RelativeTransform;
}

FTransform TransformComponent::GetWorldTransform() const
{
	return WorldTransform;
}