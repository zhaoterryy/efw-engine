#pragma once

#include "efw-engine/EngineTypes.h"
#include "GameFramework/Object.h"
#include "BaseComponent.h"

class TransformComponent : public BaseComponent
{

public :
	TransformComponent(SceneObject* inHost);

	TransformComponent(SceneObject* inHost, FTransform trans);

	TransformComponent(SceneObject* inHost, FVector pos, float rot, FVector scl);

	virtual void Tick(float deltaTime) override;

	void SetRelativeTransform(const FTransform transform);
	FTransform GetRelativeTransform() const;
	FTransform GetWorldTransform() const;

protected:
	FTransform relativeTransform;

private:
	FTransform worldTransform;

};