#include "BaseComponent.h"
#include "GameFramework/SceneObject.h"

BaseComponent::BaseComponent(SceneObject* inHost) :
	host(inHost)
{
}

void BaseComponent::Tick(float deltaTime)
{

}

SceneObject* BaseComponent::GetHost()
{
	return host;
}
