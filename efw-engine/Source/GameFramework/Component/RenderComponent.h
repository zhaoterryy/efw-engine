#pragma once

#include "BaseComponent.h"

class RenderComponent : public BaseComponent 
{
public:
    RenderComponent(SceneObject* inHost);

    std::string spriteId;
};