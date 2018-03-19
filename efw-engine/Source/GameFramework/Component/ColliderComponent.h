#pragma once

#include "BaseComponent.h"
#include "efw-engine/EngineTypes.h"

struct AABB 
{
    FVector center;
    FVector extent;

    FVector getBottomLeft() 
    {
        return FVector(center.X - extent.X, center.Y - extent.Y);
    }

    FVector getTopRight()
    {
        return FVector(center.X + extent.X, center.Y + extent.Y);
    }
};

class ColliderComponent : public BaseComponent 
{
public:
    ColliderComponent(SceneObject* inHost);

    AABB bounds;
};