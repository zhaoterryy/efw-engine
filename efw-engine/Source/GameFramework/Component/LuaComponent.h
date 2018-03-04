#pragma once
#include "BaseComponent.h"
#include "sol.hpp"

class LuaComponent : public BaseComponent
{
public:
	LuaComponent(SceneObject* inHost) : BaseComponent(inHost) {}

	virtual void Tick(float deltaTime) override;
	sol::function luaTick;
};