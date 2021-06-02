
#include "Functions.h"


float FMathFunc::Clamp(float val, float low, float high)
{
	return fmaxf(fminf(val, high), low);
}

float FMathFunc::Log2(float val)
{
	return log2(val);
}
float FMathFunc::Log4(float val)
{
	return log2(val) / 2.0f;
}

float FMathFunc::Sqrt(float val)
{
	return sqrtf(val);
}