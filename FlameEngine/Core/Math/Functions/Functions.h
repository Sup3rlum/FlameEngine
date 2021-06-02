#pragma once

#include "../MathFwdDef.h"

EXPORT(struct, FMathFunc)
{

	// Regular Trig
	FORCEINLINE static TVector2<float> Cos(TVector2<float> val);
	FORCEINLINE static TVector3<float> Cos(TVector3<float> val);
	FORCEINLINE static TVector4<float> Cos(TVector4<float> val);

	FORCEINLINE static TVector2<float> Sin(TVector2<float> val);
	FORCEINLINE static TVector3<float> Sin(TVector3<float> val);
	FORCEINLINE static TVector4<float> Sin(TVector4<float> val);


	FORCEINLINE static TVector2<float> Tan(TVector2<float> val);
	FORCEINLINE static TVector3<float> Tan(TVector3<float> val);
	FORCEINLINE static TVector4<float> Tan(TVector4<float> val);


	FORCEINLINE static float Sin(float val);
	FORCEINLINE static float Cos(float val);
	FORCEINLINE static float Tan(float val);


	// Hyperbolic trig
	FORCEINLINE static TVector2<float> Cosh(TVector2<float> val);
	FORCEINLINE static TVector3<float> Cosh(TVector3<float> val);
	FORCEINLINE static TVector4<float> Cosh(TVector4<float> val);

	FORCEINLINE static TVector2<float> Sinh(TVector2<float> val);
	FORCEINLINE static TVector3<float> Sinh(TVector3<float> val);
	FORCEINLINE static TVector4<float> Sinh(TVector4<float> val);


	FORCEINLINE static TVector2<float> Tanh(TVector2<float> val);
	FORCEINLINE static TVector3<float> Tanh(TVector3<float> val);
	FORCEINLINE static TVector4<float> Tanh(TVector4<float> val);

	FORCEINLINE static float Sinh(float val);
	FORCEINLINE static float Cosh(float val);
	FORCEINLINE static float Tanh(float val);



	FORCEINLINE static float Sqrt(float);


	// Inverses

	 


	// Other Transcendental
	FORCEINLINE static TVector2<float> Exp(TVector2<float> val);
	FORCEINLINE static TVector3<float> Exp(TVector3<float> val);
	FORCEINLINE static TVector4<float> Exp(TVector4<float> val);

	FORCEINLINE static float Exp(float val);
	FORCEINLINE static float Log2(float val);

	FORCEINLINE static float Log4(float val);

	FORCEINLINE static float Clamp(float val, float low, float high);

};