#pragma once

#include "Object.h"

class AActor : public Object
{
public:
	// Object interface
	virtual void Tick(float DeltaTime) override;

	void SetActorTransform(const FTransform InTransform);
	FTransform GetActorTransform() const;
private:
	FTransform Transform;
};