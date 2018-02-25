#include "Object.h"
#include "..\Msvc\TransformComponent.h"

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
	Components.insert(new TransformComponent(this, pos, rot, scl));
}

void Object::Tick(float DeltaTime)
{
	for (const auto& comp : Components) {
		comp->Tick(DeltaTime);
	}
}

Object* Object::GetParent()
{
	if (Parent) { return Parent; }
	else { return nullptr; }
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