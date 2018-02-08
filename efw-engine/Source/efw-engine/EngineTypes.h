#pragma once

// 2-dimensional vector
struct FVector
{
	float X;
	float Y;

	FVector() : X(0.0f), Y(0.0f) {}
	FVector(float InX, float InY) : X(InX), Y(InY) {}

	FVector(float Value)
	{
		X = Y = Value;
	}
};

struct FVector3
{
	float X;
	float Y;
	float Z;

	FVector3() : X(0), Y(0), Z(0) {}
	FVector3(float InX, float InY, float InZ) : X(InX), Y(InY), Z(InZ) {}

	// Set uniformly
	FVector3(float value)
	{
		X = Y = Z = value;
	}

	FVector3 operator+(const FVector3& vec)
	{
		FVector3 vector;
		vector.X = this->X + vec.X;
		vector.Y = this->Y + vec.Y;
		vector.Z = this->Z + vec.Z;
		return vector;
	}
};

struct FRotator
{
	float Pitch;
	float Yaw;
	float Roll;

	FRotator() : Pitch(0), Yaw(0), Roll(0) {}
	FRotator(float InPitch, float InYaw, float InRoll) : Pitch(InPitch), Yaw(InYaw), Roll(InRoll) {}

	FRotator operator+(const FRotator& rot)
	{
		FRotator rotator;
		rotator.Pitch = this->Pitch + rot.Pitch;
		rotator.Yaw = this->Yaw + rot.Yaw;
		rotator.Roll = this->Roll + rot.Roll;
		return rotator;
	}
};

struct FTransform
{
	FVector3 Position;
	FRotator Rotation;
	FVector3 Scale;

	FTransform() {}
	FTransform(FVector3 InPosition, FRotator InRotation, FVector3 InScale) : Position(InPosition), Rotation(InRotation), Scale(InScale) {}

	FTransform operator+(const FTransform& trans)
	{
		FTransform transform;
		transform.Position = this->Position + trans.Position;
		transform.Rotation = this->Rotation + trans.Rotation;
		transform.Scale = this->Scale + trans.Scale;
		return transform;
	}
};