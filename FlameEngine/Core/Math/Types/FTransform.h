#pragma once


#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

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

	FTransform(const FMatrix4& matrix)
	{
		Position = matrix[3].xyz; // Get the XYZ components of the 3rd row
		Orientation = FQuaternion(FMatrix4::ToMatrix3(matrix)); // Get the rotation component from the 3x3 submatrix

	}


	static FTransform Interp(const FTransform& t1, const FTransform& t2, float weight)
	{
		return FTransform(
			FVector3::Lerp(t1.Position, t2.Position, weight),
			FQuaternion::Slerp(t1.Orientation, t2.Orientation, weight)
		);
	}


	FORCEINLINE FMatrix4 GetMatrix() const
	{
		FRotationMatrix rot(Orientation);
		FTranslationMatrix trans(Position);

		return trans * rot;
	}
};