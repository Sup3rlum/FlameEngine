#pragma once

#include "../MathFwdDef.h"


typedef TVector2<float> FVector2;
typedef TVector3<float> FVector3;
typedef TVector4<float> FVector4;

EXPORT(struct, FMath)
{
	// Regular Trig
	FORCEINLINE static float	Cos(float val);
	FORCEINLINE static FVector2 Cos(FVector2 val);
	FORCEINLINE static FVector3 Cos(FVector3 val);
	FORCEINLINE static FVector4 Cos(FVector4 val);

	FORCEINLINE static float	Sin(float val);
	FORCEINLINE static FVector2 Sin(FVector2 val);
	FORCEINLINE static FVector3 Sin(FVector3 val);
	FORCEINLINE static FVector4 Sin(FVector4 val);

	FORCEINLINE static float	Tan(float val);
	FORCEINLINE static FVector2 Tan(FVector2 val);
	FORCEINLINE static FVector3 Tan(FVector3 val);
	FORCEINLINE static FVector4 Tan(FVector4 val);


	// Hyperbolic trig
	FORCEINLINE static FVector2 Cosh(FVector2 val);
	FORCEINLINE static FVector3 Cosh(FVector3 val);
	FORCEINLINE static FVector4 Cosh(FVector4 val);

	FORCEINLINE static FVector2 Sinh(FVector2 val);
	FORCEINLINE static FVector3 Sinh(FVector3 val);
	FORCEINLINE static FVector4 Sinh(FVector4 val);


	FORCEINLINE static FVector2 Tanh(FVector2 val);
	FORCEINLINE static FVector3 Tanh(FVector3 val);
	FORCEINLINE static FVector4 Tanh(FVector4 val);

	FORCEINLINE static float Sinh(float val);
	FORCEINLINE static float Cosh(float val);
	FORCEINLINE static float Tanh(float val);



	FORCEINLINE static float Sqrt(float val);
	FORCEINLINE static FVector2 Sqrt(FVector2 val);
	FORCEINLINE static FVector3 Sqrt(FVector3 val);
	FORCEINLINE static FVector4 Sqrt(FVector4 val);


	// Inverses

	 


	// Other Transcendental
	FORCEINLINE static FVector2 Exp(FVector2 val);
	FORCEINLINE static FVector3 Exp(FVector3 val);
	FORCEINLINE static FVector4 Exp(FVector4 val);

	FORCEINLINE static float Exp(float val);
	FORCEINLINE static float Log2(float val);

	FORCEINLINE static float Log4(float val);

	FORCEINLINE static float Clamp(float val, float low, float high);
	FORCEINLINE static FVector2 Clamp(FVector2 val, FVector2 low, FVector2 high);
	FORCEINLINE static FVector3 Clamp(FVector3 val, FVector3 low, FVector2 high);



	FORCEINLINE static float Max(float  arg)
	{
		return arg;
	}
	FORCEINLINE static float Max(float  arg, float arg2)
	{
		return fmaxf(arg, arg2);
	}

	template<typename TArg, typename... TArgs>
	FORCEINLINE static float Max(TArg arg, TArg arg2, TArgs... args)
	{
		return Max(arg, Max(arg2, args...));
	}

	/*
	template<typename TField>
	FORCEINLINE static TVector3<TField> Sign(TVector3<TField> vector)
	{
		return signVector;
	}

	*/

};