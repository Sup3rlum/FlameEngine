#include "fMatrix3.h"


fMatrix3 fMatrix3::Transpose(fMatrix3 m)
{
	fMatrix3 Result = m;

	Result[0][1] = m[1][0];
	Result[0][2] = m[2][0];

	Result[1][2] = m[2][1];
	Result[2][1] = m[1][2];

	Result[2][0] = m[0][2];
	Result[1][0] = m[0][1];

	return Result;
}