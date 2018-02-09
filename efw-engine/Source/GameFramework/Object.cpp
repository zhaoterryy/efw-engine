#include "Object.h"

Object::Object(Object* InParent /*= nullptr*/)
{
	Object(FVector(), 0.0f, FVector(), InParent);
}

Object::Object(FTransform InTransform, Object* InParent /*= nullptr*/)
{
	Object(InTransform.Position, InTransform.Rotation, InTransform.Scale);
}

Object::Object(FVector InPos, float InRot, FVector InScale, Object* InParent /*= nullptr*/)
{
	Parent = InParent;
	RelativeTransform.Position = InPos;
	RelativeTransform.Rotation = InRot;
	RelativeTransform.Scale = InScale;
}

void Object::Tick(float DeltaTime)
{
	if (Parent) {
		WorldTransform = Parent->GetWorldTransform() + RelativeTransform;
	}
	else {
		WorldTransform = RelativeTransform;
	}
}

void Object::SetRelativeTransform(const FTransform InTransform)
{
	RelativeTransform = InTransform;
}

FTransform Object::GetRelativeTransform() const
{
	return RelativeTransform;
}

FTransform Object::GetWorldTransform() const
{
	return WorldTransform;
}

void Object::SetParent(Object* InObj)
{
	Parent = InObj;
}

void Object::AddChild(Object* InObj)
{
	Children.push_back(InObj);
	InObj->SetParent(this);
}
