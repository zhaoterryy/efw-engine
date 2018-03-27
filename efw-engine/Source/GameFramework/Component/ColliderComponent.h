#pragma once

#include "BaseComponent.h"
#include "efw-engine/EngineTypes.h"

struct AABB 
{
    FVector center;
    FVector extent;

    FVector getBottomLeft() 
    {
        return FVector(center.x - extent.x, center.y - extent.y);
    }

    FVector getTopRight()
    {
        return FVector(center.x + extent.x, center.y + extent.y);
    }
};

class ColliderComponent : public BaseComponent 
{
public:
    ColliderComponent(SceneObject* inHost);

    AABB bounds;
};