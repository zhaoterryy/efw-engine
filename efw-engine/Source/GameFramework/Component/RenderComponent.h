#pragma once

#include "BaseComponent.h"
#include <string>

class RenderComponent : public BaseComponent 
{
public:
    RenderComponent(SceneObject* inHost);

    std::string spriteId;
};