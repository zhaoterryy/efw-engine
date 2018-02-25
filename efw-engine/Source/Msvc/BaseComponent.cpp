#include "BaseComponent.h"

BaseComponent::BaseComponent()
{

}

BaseComponent::BaseComponent(Object* host)
{
	Host = host;
}

void BaseComponent::Tick(float DeltaTime)
{

}

Object* BaseComponent::GetHost()
{
	return Host;
}
