#pragma once


#include "TMatrix4.h"
#include "Vector.h"
#include "Quaternion.h"



typedef TMatrix4<float> FMatrix4;
typedef TMatrix3<float> FMatrix3;
typedef TMatrix2<float> FMatrix2;
		
typedef TMatrix4<double> DMatrix4;
typedef TMatrix3<double> DMatrix3;
typedef TMatrix2<double> DMatrix2;
		 
typedef TMatrix4<int> IMatrix4;
typedef TMatrix3<int> IMatrix3;
typedef TMatrix2<int> IMatrix2;
		 
typedef TMatrix4<long long> LMatrix4;
typedef TMatrix3<long long> LMatrix3;
typedef TMatrix2<long long> LMatrix2;
		
typedef TMatrix4<TCmplx<float>> CMatrix4;
typedef TMatrix3<TCmplx<float>> CMatrix3;
typedef TMatrix2<TCmplx<float>> CMatrix2;




EXPORT(class, FScalingMatrix) : public FMatrix4
{
public:


	FScalingMatrix(float sx, float sy, float sz) :
		FMatrix4(
			FVector4(sx,0,0,0),
			FVector4(0,sy,0,0),
			FVector4(0,0,sz,0),
			FVector4(0,0,0,1)
		)
	{}

	FScalingMatrix(float f) :
		FScalingMatrix(f,f,f)
	{}


	FScalingMatrix(FVector3 scale) :
		FScalingMatrix(scale.x,scale.y,scale.z)
	{}
};


EXPORT(class, FTranslationMatrix) : public FMatrix4
{
public:
	FTranslationMatrix(float x, float y, float z) :
		FMatrix4(
			FVector4(1,0,0,0),
			FVector4(0,1,0,0),
			FVector4(0,0,1,0),
			FVector4(x,y,z,1)
		)
	{}

	FTranslationMatrix(FVector3 vector) :
		FTranslationMatrix(vector.x, vector.y, vector.z)
	{}
};

EXPORT(class, FRotationMatrix) : public FMatrix4
{
public:
	FRotationMatrix(float rotx, float roty, float rotz)
	{
			float sx = sinf(rotx);
			float cx = cosf(rotx);

			float sy = sinf(roty);
			float cy = cosf(roty);

			float sz = sinf(rotz);
			float cz = cosf(rotz);

			*this = 
				FMatrix4
				(
					FVector4(cz, -sz, 0, 0),
					FVector4(sz, cz, 0, 0),
					FVector4(0, 0, 1, 0),
					FVector4(0, 0, 0, 1)
				)

				*
				FMatrix4
				(
					FVector4(1, 0, 0, 0),
					FVector4(0, cx, -sx, 0),
					FVector4(0, sx, cx, 0),
					FVector4(0, 0, 0, 1)
				);
	}
	FRotationMatrix(FQuaternion q) :
		FMatrix4
		(
			FVector4(1 - 2 * (q.j * q.j + q.k * q.k),	2 * (q.i * q.j + q.k * q.r),		2 * (q.i * q.k - q.j * q.r), 0),
			FVector4(2 * (q.i * q.j - q.k * q.r),		1 - 2 * (q.i * q.i + q.k * q.k),	2 * (q.j * q.k + q.i * q.r), 0),
			FVector4(2 * (q.i * q.k + q.r * q.j),		2 * (q.j * q.k - q.i * q.r),		1 - 2 * (q.i * q.i + q.j * q.j), 0),
			FVector4(0, 0, 0, 1)
		)
	{}
	FRotationMatrix(const FMatrix4& other) : FMatrix4(other) {}
	FRotationMatrix() : FMatrix4() {}

};


EXPORT(class, FReflectionMatrix) : public FMatrix4
{

};


EXPORT(class, FViewMatrix) : public FMatrix4
{
public:
	FViewMatrix(const FVector3& eyePos, const FVector3& eyeTarget, const FVector3& camUp)
	{
		FVector3 f(FVector3::Normalize(eyeTarget - eyePos));
		FVector3 s(FVector3::Normalize(f ^ camUp));
		FVector3 u(s ^ f);

		FMatrix4 Result(1);
		Result[0][0] = s.x;
		Result[1][0] = s.y;
		Result[2][0] = s.z;
		Result[0][1] = u.x;
		Result[1][1] = u.y;
		Result[2][1] = u.z;
		Result[0][2] = -f.x;
		Result[1][2] = -f.y;
		Result[2][2] = -f.z;
		Result[3][0] = -FVector3::Dot(s, eyePos);
		Result[3][1] = -FVector3::Dot(u, eyePos);
		Result[3][2] = FVector3::Dot(f, eyePos);

		*this = Result;
	}

	FViewMatrix(FVector3 eyePos, FQuaternion orientation) : FMatrix4(FRotationMatrix(FQuaternion::Inverse(orientation))* FTranslationMatrix(-eyePos)) {}
	FViewMatrix(const FMatrix4& other) : FMatrix4(other) {}
	FViewMatrix() : FMatrix4() {}
};



EXPORT(class, FProjectionMatrix) : public FMatrix4
{
public:

	FProjectionMatrix(const FMatrix4& other) : FMatrix4(other) {}
	FProjectionMatrix() : FMatrix4() {}
};


EXPORT(class, FPerspectiveMatrix) : public FProjectionMatrix
{
public:
	FPerspectiveMatrix(float fovy, float aspect, float zNear, float zFar)
	{
		assert(abs(aspect - std::numeric_limits<float>::epsilon()) > 0);

		float const tanHalfFovy = tan(fovy / 2.0f);

		FMatrix4 Result(0);
		Result[0][0] = 1.0f / (aspect * tanHalfFovy);
		Result[1][1] = 1.0f / (tanHalfFovy);
		Result[2][2] = -(zFar + zNear) / (zFar - zNear);
		Result[2][3] = -1.0f;
		Result[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
		*this = Result;
	}

	FPerspectiveMatrix(const FMatrix4& other) : FProjectionMatrix(other) {}
	FPerspectiveMatrix(const FPerspectiveMatrix& other) : FProjectionMatrix(other) {}
	FPerspectiveMatrix() : FProjectionMatrix() {}
};

EXPORT(class, FOrthographicMatrix) : public FProjectionMatrix
{
public:
	FOrthographicMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		FMatrix4 Result(1);
		Result[0][0] = 2.0f / (right - left);
		Result[1][1] = 2.0f / (top - bottom);
		Result[2][2] = 1.0f / (zNear - zFar);
		Result[3][0] = (left + right) / (left - right);
		Result[3][1] = (top + bottom) / (bottom - top);
		Result[3][2] = zNear / (zFar - zNear);

		*this = Result;
	}

	FOrthographicMatrix(const FMatrix4& other) : FProjectionMatrix(other) {}
	FOrthographicMatrix(const FOrthographicMatrix& other) : FProjectionMatrix(other) {}
	FOrthographicMatrix() : FProjectionMatrix() {}
};


EXPORT(class, FBasisMatrix) : public FMatrix4
{
public:
	FBasisMatrix(FVector3 b1, FVector3 b2, FVector3 b3) :
		FMatrix4
		(
			FVector4(b1.x, b2.x, b3.x, 0),
			FVector4(b1.y, b2.y, b3.y, 0),
			FVector4(b1.z, b2.z, b3.z, 0),
			FVector4(0, 0, 0, 1)
		)
	{

	}
};


EXPORT(class, FOrthonormalBasisMatrix) : public FBasisMatrix
{
public:
	FOrthonormalBasisMatrix(FVector3 a, FVector3 b) :
		FBasisMatrix(FVector3(0),FVector3(0), FVector3(0))
	{
		/*
		FVector3 c = FVector3::Normalize(a) ^ FVector3::Normalize(b);
		FVector3 b1 = c ^ a;

		*this = FBasisMatrix(a, b1, c);*/
	}
};

FORCEINLINE FLAME_API FVector3 operator*(const FRotationMatrix& rotation, const FVector3& vec)
{
	return (rotation * FVector4(vec, 1.0f)).xyz;
}