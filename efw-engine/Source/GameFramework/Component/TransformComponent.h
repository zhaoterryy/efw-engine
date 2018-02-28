#pragma once

#include "efw-engine/EngineTypes.h"
#include "GameFramework/Object.h"
#include "BaseComponent.h"

class TransformComponent : public BaseComponent
{

public :
	TransformComponent(Object* host);

	TransformComponent(Object* host, FTransform trans);

	TransformComponent(Object* host, FVector3 pos, FRotator rot, FVector3 scl);

	virtual void Tick(float DeltaTime);

	void SetRelativeTransform(const FTransform InTransform);
	FTransform GetRelativeTransform() const;
	FTransform GetWorldTransform() const;

protected:
	FTransform RelativeTransform;

private:
	FTransform WorldTransform;

};