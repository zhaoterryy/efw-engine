#pragma once

#include "efw-engine/EngineTypes.h"

class Object
{
public:
	Object() = default;

	virtual void Tick(float deltaTime);

	inline std::string GetName() { return name; }

	void SetName(std::string inName);

private:
	std::string name;
};