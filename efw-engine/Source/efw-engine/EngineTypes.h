#pragma once

struct FVector3
{
	float X;
	float Y;
	float Z;

	FVector3(float InX, float InY, float InZ) : X(InX), Y(InY), Z(InZ) {}

	// Set uniformly
	FVector3(float value)
	{
		X = Y = Z = value;
	}
};

struct FRotator
{
	float Pitch;
	float Yaw;
	float Roll;

	FRotator(float InPitch, float InYaw, float InRoll) : Pitch(InPitch), Yaw(InYaw), Roll(InRoll) {}
};

struct FTransform
{
	FVector3 Position;
	FRotator Rotation;
	FVector3 Scale;

	FTransform(FVector3 InPosition, FRotator InRotation, FVector3 InScale) : Position(InPosition), Rotation(InRotation), Scale(InScale) {}
};
