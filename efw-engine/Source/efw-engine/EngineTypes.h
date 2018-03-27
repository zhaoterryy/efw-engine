#pragma once
#include <iostream>

// 2-dimensional vector
struct FVector
{
	float x;
	float y;

	FVector() : x(0.0f), y(0.0f) {}
	FVector(float _x, float _y) : x(_x), y(_y) {}

	FVector(float value)
	{
		x = y = value;
	}
	
	friend FVector operator+(const FVector& lhs, const FVector& rhs)
	{
		return FVector(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	friend FVector operator*(const FVector& lhs, const FVector& rhs)
	{
		return FVector(lhs.x * rhs.x, lhs.y * rhs.y);
	}

	friend std::ostream& operator<<(std::ostream& os, const FVector& vector)
	{
		os << "X: " << vector.x << " Y: " << vector.y;
		return os;
	}
};

struct FVector3D
{
	float x;
	float y;
	float z;

	FVector3D() : x(0), y(0), z(0) {}
	FVector3D(float InX, float InY, float InZ) : x(InX), y(InY), z(InZ) {}

	// Set uniformly
	FVector3D(float value)
	{
		x = y = z = value;
	}

	FVector3D operator+(const FVector3D& vec)
	{
		FVector3D vector;
		vector.x = this->x + vec.x;
		vector.y = this->y + vec.y;
		vector.z = this->z + vec.z;
		return vector;
	}

	friend std::ostream& operator<<(std::ostream& os, const FVector3D& Vector)
	{
		os << "X: " << Vector.x << " Y: " << Vector.y << " Z: " << Vector.z;
		return os;
	}
};

struct FRotator
{
	float pitch;
	float yaw;
	float roll;

	FRotator() : pitch(0), yaw(0), roll(0) {}
	FRotator(float InPitch, float InYaw, float InRoll) : pitch(InPitch), yaw(InYaw), roll(InRoll) {}

	FRotator operator+(const FRotator& rot)
	{
		FRotator rotator;
		rotator.pitch = this->pitch + rot.pitch;
		rotator.yaw = this->yaw + rot.yaw;
		rotator.roll = this->roll + rot.roll;
		return rotator;
	}

	friend std::ostream& operator<<(std::ostream& os, const FRotator& Rotator)
	{
		os << "Pitch: " << Rotator.pitch << " Yaw: " << Rotator.yaw << " Roll: " << Rotator.roll;
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
	FVector position;
	float rotation;
	FVector scale;

	FTransform() = default;
	FTransform(FVector inPos, float inRot, FVector inScale) : position(inPos), rotation(inRot), scale(inScale) {}

	friend FTransform operator+(const FTransform& lhs, const FTransform& rhs)
	{
		return FTransform(lhs.position + rhs.position,
			lhs.rotation + rhs.rotation,
			lhs.scale * rhs.scale);
	}

	friend std::ostream& operator<<(std::ostream& os, const FTransform& transform)
	{
		os << "\nPosition: " << transform.position << "\nRotation: " << transform.rotation << "\nScale: " << transform.scale << std::endl;
		return os;
	}
};