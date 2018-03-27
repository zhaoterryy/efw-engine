#include "RenderComponent.h"

RenderComponent::RenderComponent(SceneObject* inHost) :
    BaseComponent(inHost)
{
}

RenderComponent::RenderComponent(SceneObject* inHost, const std::string& id) :
    BaseComponent(inHost),
    spriteId(id)
{
}
