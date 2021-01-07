#pragma once


#include "FVector3.h"
#include "FQuaternion.h"
#include "FMatrix.h"

EXPORT(class, FTransform)
{
public:
	FVector3 Position;
	FQuaternion Orientation;
	
	FTransform(const FTransform& other) :
		Position(other.Position),
		Orientation(other.Orientation)
	{

	}


	FTransform(const FVector3& Position) :
		Position(Position),
		Orientation(FQuaternion::Identity())
	{

	}


	FTransform(const FQuaternion& Orientation) :
		Position(FVector3(0)),
		Orientation(Orientation)
	{

	}

	FTransform(const FVector3& Position, const FQuaternion& Orientation) :
		Position(Position),
		Orientation(Orientation)
	{

	}

	FTransform() :
		Position(FVector3(0)),
		Orientation(FQuaternion::Identity())
	{

	}


	FORCEINLINE FMatrix4 GetMatrix() const
	{
		return FRotationMatrix(Orientation) * FTranslationMatrix(Position);
	}
};