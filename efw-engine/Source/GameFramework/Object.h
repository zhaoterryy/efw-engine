#pragma once

#include "efw-engine/EngineTypes.h"
#include <vector>

class Object
{

public:
	Object();

	virtual void Tick(float DeltaTime);


public:
	void SetRelativeTransform(const FTransform InTransform);
	FTransform GetRelativeTransform() const;
	FTransform GetWorldTransform() const;

	void SetParent(Object* obj);
	void AddChild(Object* obj);

protected:
	FTransform RelativeTransform;

	Object* Parent;
	std::vector<Object*> Children;


private:
	FTransform WorldTransform;

};