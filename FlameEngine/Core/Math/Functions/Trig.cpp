#include "Functions.h"

#include <cmath>
#include "../Types/Vector.h"


FVector3 FMathFunc::Sin(FVector3 val)
{
	return FVector3
	(
		Sin(val.x),
		Sin(val.y),
		Sin(val.z)
	);
}

FVector3 FMathFunc::Cos(FVector3 val)
{
	return FVector3
	(
		Cos(val.x),
		Cos(val.y),
		Cos(val.z)
	);
}



float FMathFunc::Sin(float val)
{
	return ::sinf(val);
}
float FMathFunc::Cos(float val)
{
	return ::cosf(val);
}