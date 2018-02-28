#include "BaseComponent.h"

BaseComponent::BaseComponent()
{

}

BaseComponent::BaseComponent(Object* InHost)
{
	Host = InHost;
}

void BaseComponent::Tick(float DeltaTime)
{

}

Object* BaseComponent::GetHost()
{
	return Host;
}
