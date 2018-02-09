#pragma once

#include "efw-engine/EngineTypes.h"
#include <vector>

class Object
{

public:
	Object(Object* InParent = nullptr);

	Object(FTransform InTransform, Object* InParent = nullptr);

	Object(FVector InPos, float InRot, FVector InScale, Object* InParent = nullptr);

	virtual void Tick(float DeltaTime);

public:
	void SetRelativeTransform(const FTransform InTransform);
	FTransform GetRelativeTransform() const;
	FTransform GetWorldTransform() const;

	void SetParent(Object* InObj);
	void AddChild(Object* InObj);

protected:
	FTransform RelativeTransform;

	Object* Parent;
	std::vector<Object*> Children;


private:
	FTransform WorldTransform;

};