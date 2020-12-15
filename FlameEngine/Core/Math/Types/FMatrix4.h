#pragma once

#include "Core/Common/CoreCommon.h"
#include "../Geometry/fPlane.h"
#include "FVector4.h"
#include "FMatrix3.h"
#include "FQuaternion.h"

EXPORT(class, FMatrix4)
{

	FVector4 value[4];

public:

		
	constexpr FMatrix4() : 
		value
	{ 
		FVector4(0), 
		FVector4(0), 
		FVector4(0), 
		FVector4(0) 
	} {}

	constexpr FMatrix4(float s) : 
		value
	{ 
		FVector4(s, 0, 0, 0), 
		FVector4(0, s, 0, 0),
		FVector4(0, 0, s, 0), 
		FVector4(0, 0, 0, s) 
	} {}

	constexpr FMatrix4(
		FVector4 const& v1, 
		FVector4 const& v2, 
		FVector4 const& v3, 
		FVector4 const& v4) : 
		value
	{ 
		FVector4(v1), 
		FVector4(v2), 
		FVector4(v3), 
		FVector4(v4) 
	} {}


	constexpr FMatrix4(
		FMatrix3 const& m) :
		value
	{
		FVector4(m[0], 0),
		FVector4(m[1], 0),
		FVector4(m[2], 0),
		FVector4(0)
	} {}

	static FMatrix3 ToMatrix3(FMatrix4 _m);
	static FMatrix4 Transpose(FMatrix4 _m);

	static FMatrix4 Identity();
	static float Determinant(FMatrix4 _m);
	static FMatrix4 Inverse(FMatrix4 _m);

	static FMatrix4 FromQuaternion(FQuaternion const& q);
	static FMatrix4 FromEulerAngles(float _rotX, float _rotY, float _rotZ);

	static FMatrix4 Scaling(float scaleX, float scaleY, float scaleZ);
	static FMatrix4 Scaling(FVector3 _scale);
	static FMatrix4 Scaling(float scale);


	static FMatrix4 Translation(float transX, float transY, float transZ);
	static FMatrix4 Translation(FVector3 _trans);
	static FMatrix4 Reflection(FPlane _plane);

	static FMatrix4 CreateView(FVector3 eyePos, FVector3 eyeTarget, FVector3 camUp);
	static FMatrix4 CreatePerspective(float fov, float aspectRatio, float zNear, float zFar);
	static FMatrix4 CreateOrthographic(float left, float right, float bottom, float top, float zNear, float zFar);


	FVector4& operator[](size_t _index)
	{
		assert(_index < 4);

		return value[_index];
	}


	constexpr FVector4 const& operator[](size_t _index) const
	{
		assert(_index < 4);

		return value[_index];
	}



	constexpr FMatrix4& operator=(FMatrix4 const& m)
	{
		this->value[0] = m[0];
		this->value[1] = m[1];
		this->value[2] = m[2];
		this->value[3] = m[3];

		return *this;
	}

};

FLAME_API FMatrix4 const& operator*(float const& l, FMatrix4 const& r);
FLAME_API FMatrix4 const& operator*(FMatrix4 const& l, float const& r);


FLAME_API FVector4	 operator*(FMatrix4 const& l, FVector4 const& r);
FLAME_API FMatrix4& operator*(FMatrix4 const& l, FMatrix4 const& r);