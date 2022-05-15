
#include <cmath>

#include "Functions.h"
#include "../Types/Vector.h"

float FMath::Sin(float val)
{
	return ::sinf(val);
}

FVector2 FMath::Sin(FVector2 val)
{
	return FVector2
	(
		Sin(val.x),
		Sin(val.y)
	);
}

FVector3 FMath::Sin(FVector3 val)
{
	return FVector3
	(
		Sin(val.x),
		Sin(val.y),
		Sin(val.z)
	);
}



float FMath::Cos(float val)
{
	return ::cosf(val);
}

FVector2 FMath::Cos(FVector2 val)
{
	return FVector2
	(
		Cos(val.x),
		Cos(val.y)
	);
}


FVector3 FMath::Cos(FVector3 val)
{
	return FVector3
	(
		Cos(val.x),
		Cos(val.y),
		Cos(val.z)
	);
}

