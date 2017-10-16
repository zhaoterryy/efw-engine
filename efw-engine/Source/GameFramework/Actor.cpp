#include "Actor.h"

void AActor::Tick(float DeltaTime)
{

}

void AActor::SetActorTransform(const FTransform InTransform)
{
	Transform = InTransform;
}

FTransform AActor::GetActorTransform() const
{
	return Transform;
}

