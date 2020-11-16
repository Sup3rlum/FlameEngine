#pragma once

#include "../../dll/nchfx.h"
#include "../Geometry/fPlane.h"
#include "fVector4.h"
#include "fMatrix3.h"
#include "fQuaternion.h"

EXPORT_STRUCT fMatrix4
{

	fVector4 value[4];

public:

		
	constexpr fMatrix4() : 
		value
	{ 
		fVector4(0), 
		fVector4(0), 
		fVector4(0), 
		fVector4(0) 
	} {}

	constexpr fMatrix4(float s) : 
		value
	{ 
		fVector4(s, 0, 0, 0), 
		fVector4(0, s, 0, 0),
		fVector4(0, 0, s, 0), 
		fVector4(0, 0, 0, s) 
	} {}

	constexpr fMatrix4(
		fVector4 const& v1, 
		fVector4 const& v2, 
		fVector4 const& v3, 
		fVector4 const& v4) : 
		value
	{ 
		fVector4(v1), 
		fVector4(v2), 
		fVector4(v3), 
		fVector4(v4) 
	} {}


	constexpr fMatrix4(
		fMatrix3 const& m) :
		value
	{
		fVector4(m[0], 0),
		fVector4(m[1], 0),
		fVector4(m[2], 0),
		fVector4(0)
	} {}

	static fMatrix3 ToMatrix3(fMatrix4 _m);
	static fMatrix4 Transpose(fMatrix4 _m);

	static fMatrix4 Identity();
	static float Determinant(fMatrix4 _m);
	static fMatrix4 Inverse(fMatrix4 _m);

	static fMatrix4 FromQuaternion(fQuaternion const& q);
	static fMatrix4 FromEulerAngles(float _rotX, float _rotY, float _rotZ);

	static fMatrix4 Scaling(float scaleX, float scaleY, float scaleZ);
	static fMatrix4 Scaling(fVector3 _scale);
	static fMatrix4 Scaling(float scale);


	static fMatrix4 Translation(float transX, float transY, float transZ);
	static fMatrix4 Translation(fVector3 _trans);
	static fMatrix4 Reflection(fPlane _plane);

	static fMatrix4 CreateView(fVector3 eyePos, fVector3 eyeTarget, fVector3 camUp);
	static fMatrix4 CreatePerspective(float fov, float aspectRatio, float zNear, float zFar);
	static fMatrix4 CreateOrthographic(float left, float right, float bottom, float top, float zNear, float zFar);


	fVector4& operator[](FL_INT32 _index)
	{
		assert(_index < 4);

		return value[_index];
	}


	constexpr fVector4 const& operator[](FL_INT32 _index) const
	{
		assert(_index < 4);

		return value[_index];
	}



	constexpr fMatrix4& operator=(fMatrix4 const& m)
	{
		this->value[0] = m[0];
		this->value[1] = m[1];
		this->value[2] = m[2];
		this->value[3] = m[3];

		return *this;
	}

};

FLAME_DLL fMatrix4 const& operator*(float const& l, fMatrix4 const& r);
FLAME_DLL fMatrix4 const& operator*(fMatrix4 const& l, float const& r);


FLAME_DLL fVector4 const& operator*(fMatrix4 const& l, fVector4 const& r);
FLAME_DLL fMatrix4& operator*(fMatrix4 const& l, fMatrix4 const& r);