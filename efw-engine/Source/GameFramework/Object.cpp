#include "Object.h"

void Object::Tick(float deltaTime)
{

}

void Object::SetName(std::string inName)
{
	name = inName;
}

void Object::SetScene(Scene* scene)
{
	this->scene = scene;
}
