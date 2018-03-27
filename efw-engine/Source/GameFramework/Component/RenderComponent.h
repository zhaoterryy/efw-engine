#pragma once

#include "BaseComponent.h"
#include <string>

class RenderComponent : public BaseComponent 
{
public:
    RenderComponent(SceneObject* inHost);
    RenderComponent(SceneObject* inHost, const std::string& id);

    std::string spriteId;
};