#include "Object.h"

Object::Object()
{
	Object(FVector3(), FRotator(), FVector3());
}

Object::Object(FTransform trans)
{
	Object(trans.Position, trans.Rotation, trans.Scale);
}

Object::Object(FVector3 pos, FRotator rot, FVector3 scl)
{
	Parent = nullptr;
	RelativeTransform.Position = pos;
	RelativeTransform.Rotation = rot;
	RelativeTransform.Scale = scl;
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

void Object::SetParent(Object* obj)
{
	Parent = obj;
}

void Object::AddChild(Object* obj)
{
	Children.push_back(obj);
	obj->SetParent(this);
}
