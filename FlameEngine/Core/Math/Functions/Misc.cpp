
#include "Functions.h"


float FMath::Clamp(float val, float low, float high)
{
	return fmaxf(fminf(val, high), low);
}

float FMath::Log2(float val)
{
	return log2(val);
}
float FMath::Log4(float val)
{
	return log2(val) / 2.0f;
}

float FMath::Sqrt(float val)
{
	return sqrtf(val);
}