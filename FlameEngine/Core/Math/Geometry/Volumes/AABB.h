#pragma once

#include "Core/Common/CoreCommon.h"
#include "../../Types/Vector.h"

EXPORT(class, AABB)
{
public:
	FVector3 minPoint;
	FVector3 maxPoint;

	AABB();
	AABB(FVector3 min, FVector3 max);

	void Enclose(FVector3 point);
	void Enclose(const FArray<FVector3>& point);

	FORCEINLINE float LengthX() const;
	FORCEINLINE float LengthY() const;
	FORCEINLINE float LengthZ() const;

	FORCEINLINE FVector3 Center() const;

	FORCEINLINE float Volume() const;

	void GetCorners(FStaticArray<FVector3, 8>& out);

	static AABB FromPointArray(const FArray<FVector3>&points);



	void SetDegenerate();
};

