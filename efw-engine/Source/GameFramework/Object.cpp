#include "Object.h"

Object::Object()
{
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