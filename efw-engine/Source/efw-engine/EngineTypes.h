#pragma once
#include <iostream>

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
	
	friend FVector operator+(const FVector& Lhs, const FVector& Rhs)
	{
		return FVector(Lhs.X + Rhs.X, Lhs.Y + Rhs.Y);
	}

	friend std::ostream& operator<<(std::ostream& os, const FVector& Vector)
	{
		os << "X: " << Vector.X << " Y: " << Vector.Y;
		return os;
	}
};

struct FVector3D
{
	float X;
	float Y;
	float Z;

	FVector3D() : X(0), Y(0), Z(0) {}
	FVector3D(float InX, float InY, float InZ) : X(InX), Y(InY), Z(InZ) {}

	// Set uniformly
	FVector3D(float value)
	{
		X = Y = Z = value;
	}

	FVector3D operator+(const FVector3D& vec)
	{
		FVector3D vector;
		vector.X = this->X + vec.X;
		vector.Y = this->Y + vec.Y;
		vector.Z = this->Z + vec.Z;
		return vector;
	}

	friend std::ostream& operator<<(std::ostream& os, const FVector3D& Vector)
	{
		os << "X: " << Vector.X << " Y: " << Vector.Y << " Z: " << Vector.Z;
		return os;
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

	friend std::ostream& operator<<(std::ostream& os, const FRotator& Rotator)
	{
		os << "Pitch: " << Rotator.Pitch << " Yaw: " << Rotator.Yaw << " Roll: " << Rotator.Roll;
		return os;
	}
};

struct FTransform3D
{
	FVector3D Position;
	FRotator Rotation;
	FVector3D Scale;

	FTransform3D() = default;
	FTransform3D(FVector3D InPos, FRotator InRot, FVector3D InScale) : Position(InPos), Rotation(InRot), Scale(InScale) {}

	FTransform3D operator+(const FTransform3D& trans)
	{
		FTransform3D transform;
		transform.Position = this->Position + trans.Position;
		transform.Rotation = this->Rotation + trans.Rotation;
		transform.Scale = this->Scale + trans.Scale;
		return transform;
	}

	friend std::ostream& operator<<(std::ostream& os, const FTransform3D& Transform)
	{
		os << "\nPosition: " << Transform.Position << "\nRotation: " << Transform.Rotation << "\nScale: " << Transform.Scale << std::endl;
		return os;
	}
};

struct FTransform
{
	FVector Position;
	float Rotation;
	FVector Scale;

	FTransform() = default;
	FTransform(FVector InPos, float InRot, FVector InScale) : Position(InPos), Rotation(InRot), Scale(InScale) {}

	friend FTransform operator+(const FTransform& Lhs, const FTransform& Rhs)
	{
		return FTransform(Lhs.Position + Rhs.Position,
			Lhs.Rotation + Rhs.Rotation,
			Lhs.Scale + Rhs.Scale);
	}

	friend std::ostream& operator<<(std::ostream& os, const FTransform& Transform)
	{
		os << "\nPosition: " << Transform.Position << "\nRotation: " << Transform.Rotation << "\nScale: " << Transform.Scale << std::endl;
		return os;
	}
};