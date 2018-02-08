#include "Object.h"

Object::Object()
{
	Parent = nullptr;
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
