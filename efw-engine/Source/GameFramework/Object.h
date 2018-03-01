#pragma once

#include "efw-engine/EngineTypes.h"

class Object
{
public:
	Object() = default;

	virtual void Tick(float deltaTime);
};