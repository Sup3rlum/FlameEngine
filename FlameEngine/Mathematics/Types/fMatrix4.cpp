#include "FMatrix4.h"


FMatrix4 FMatrix4::Identity()
{


	return FMatrix4
	(
		FVector4(1, 0, 0, 0),
		FVector4(0, 1, 0, 0),
		FVector4(0, 0, 1, 0),
		FVector4(0, 0, 0, 1)
	);
}


float FMatrix4::Determinant(FMatrix4 m)
{
	return
		m[0][3] * m[1][2] * m[2][1] * m[3][0] - m[0][2] * m[1][3] * m[2][1] * m[3][0] -
		m[0][3] * m[1][1] * m[2][2] * m[3][0] + m[0][1] * m[1][3] * m[2][2] * m[3][0] +
		m[0][2] * m[1][1] * m[2][3] * m[3][0] - m[0][1] * m[1][2] * m[2][3] * m[3][0] -
		m[0][3] * m[1][2] * m[2][0] * m[3][1] + m[0][2] * m[1][3] * m[2][0] * m[3][1] +
		m[0][3] * m[1][0] * m[2][2] * m[3][1] - m[0][0] * m[1][3] * m[2][2] * m[3][1] -
		m[0][2] * m[1][0] * m[2][3] * m[3][1] + m[0][0] * m[1][2] * m[2][3] * m[3][1] +
		m[0][3] * m[1][1] * m[2][0] * m[3][2] - m[0][1] * m[1][3] * m[2][0] * m[3][2] -
		m[0][3] * m[1][0] * m[2][1] * m[3][2] + m[0][0] * m[1][3] * m[2][1] * m[3][2] +
		m[0][1] * m[1][0] * m[2][3] * m[3][2] - m[0][0] * m[1][1] * m[2][3] * m[3][2] -
		m[0][2] * m[1][1] * m[2][0] * m[3][3] + m[0][1] * m[1][2] * m[2][0] * m[3][3] +
		m[0][2] * m[1][0] * m[2][1] * m[3][3] - m[0][0] * m[1][2] * m[2][1] * m[3][3] -
		m[0][1] * m[1][0] * m[2][2] * m[3][3] + m[0][0] * m[1][1] * m[2][2] * m[3][3];
}


FMatrix4 FMatrix4::Transpose(FMatrix4 m)
{
	FMatrix4 Result = m;

	Result[0][1] = m[1][0];
	Result[0][2] = m[2][0];

	Result[1][2] = m[2][1];
	Result[2][1] = m[1][2];

	Result[2][0] = m[0][2];
	Result[1][0] = m[0][1];


	Result[0][3] = m[3][0];
	Result[1][3] = m[3][1];
	Result[2][3] = m[3][2];

	Result[3][0] = m[0][3];
	Result[3][1] = m[1][3];
	Result[3][2] = m[2][3];

	return Result;
}

FMatrix4 FMatrix4::Inverse(FMatrix4 m)
{
	float Coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	float Coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
	float Coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
	
	float Coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	float Coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	float Coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];
	
	float Coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	float Coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
	float Coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	
	float Coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	float Coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
	float Coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];
	
	float Coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	float Coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
	float Coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
	
	float Coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	float Coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
	float Coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

	FVector4 Fac0(Coef00, Coef00, Coef02, Coef03);
	FVector4 Fac1(Coef04, Coef04, Coef06, Coef07);
	FVector4 Fac2(Coef08, Coef08, Coef10, Coef11);
	FVector4 Fac3(Coef12, Coef12, Coef14, Coef15);
	FVector4 Fac4(Coef16, Coef16, Coef18, Coef19);
	FVector4 Fac5(Coef20, Coef20, Coef22, Coef23);
	
	FVector4 Vec0(m[1][0], m[0][0], m[0][0], m[0][0]);
	FVector4 Vec1(m[1][1], m[0][1], m[0][1], m[0][1]);
	FVector4 Vec2(m[1][2], m[0][2], m[0][2], m[0][2]);
	FVector4 Vec3(m[1][3], m[0][3], m[0][3], m[0][3]);
	
	FVector4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
	FVector4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
	FVector4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
	FVector4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);
	
	FVector4 SignA(+1, -1, +1, -1);
	FVector4 SignB(-1, +1, -1, +1);

	FMatrix4 Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

	FVector4 Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

	FVector4  Dot0(m[0] * Row0);
	float Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

	float OneOverDeterminant = 1.0f / Dot1;

	return Inverse * OneOverDeterminant;
}


FMatrix3 FMatrix4::ToMatrix3(FMatrix4 m)
{
	return FMatrix3
	(
		m[0].xyz,
		m[1].xyz,
		m[2].xyz
	);
}


// ----- ROTATION ----------------------------------------------------------

FMatrix4 FMatrix4::FromQuaternion(FQuaternion const& quat)
{

	FQuaternion q = FQuaternion::Normalize(quat);

	return FMatrix4
	(
		FVector4(1 - 2 * (q.j * q.j + q.k * q.k), 2 * (q.i * q.j - q.k * q.r), 2 * (q.i * q.k + q.j * q.r), 0),
		FVector4(2 * (q.i * q.j + q.k * q.r), 1 - 2 * (q.i * q.i + q.k * q.k), 2 * (q.j * q.k - q.i * q.r), 0),
		FVector4(2 * (q.i * q.k - q.r * q.j), 2 * (q.j * q.k + q.i * q.r), 1 - 2 * (q.i * q.i + q.j * q.j), 0),
		FVector4(0, 0, 0, 1)
	);
}

FMatrix4 FMatrix4::FromEulerAngles(float x, float y, float z)
{

	float sx = sinf(x);
	float cx = cosf(x);

	float sy = sinf(y);
	float cy = cosf(y);

	float sz = sinf(z);
	float cz = cosf(z);

	return FMatrix4
		(
			FVector4(cz, -sz, 0, 0),
			FVector4(sz, cz, 0, 0),
			FVector4(0, 0, 1, 0),
			FVector4(0, 0, 0, 1)
		) 
		/**
		FMatrix4
		(
			FVector4(cy,	0,	-sy,	0),
			FVector4(0,	1,	0,		0),
			FVector4(sy,	0,	cy,		0),
			FVector4(0,	0,	0,		1)
		)*/
		*
		FMatrix4
		(
			FVector4(1, 0, 0, 0),
			FVector4(0, cx, -sx, 0),
			FVector4(0, sx, cx, 0),
			FVector4(0, 0, 0, 1)
		);

}




// ---- SCALING--------------------------------------------------------------

FMatrix4 FMatrix4::Scaling(float x, float y, float z)
{


	return FMatrix4
	(
		FVector4(x, 0, 0, 0),
		FVector4(0, y, 0, 0),
		FVector4(0, 0, z, 0),
		FVector4(0, 0, 0, 1)
	);
}
FMatrix4 FMatrix4::Scaling(float s)
{


	return Scaling(s, s, s);
}

FMatrix4 FMatrix4::Scaling(FVector3 v)
{


	return Scaling(v.x, v.y, v.z);
}


// ---- TRANSLATION---------------------------------------------------------

FMatrix4 FMatrix4::Translation(float x, float y, float z)
{


	return FMatrix4
	(
		FVector4(1, 0, 0, 0),
		FVector4(0, 1, 0, 0),
		FVector4(0, 0, 1, 0),
		FVector4(x, y, z, 1)
	);
}
FMatrix4 FMatrix4::Translation(FVector3 _v)
{
	return Translation(_v.x, _v.y, _v.z);
}


// ------ REFLECTION ------------------------------------------------------------


FMatrix4 FMatrix4::Reflection(FPlane _plane)
{


	return FMatrix4::Identity();
}


// -------- CAMERA ------------------------------------------------------------

FMatrix4 FMatrix4::CreateView(FVector3 eyePos, FVector3 eyeTarget, FVector3 camUp)
{
	FVector3 const f(FVector3::Normalize(eyeTarget - eyePos));
	FVector3 const s(FVector3::Normalize(f ^ camUp));
	FVector3 const u(s ^ f);

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
	return Result;
}


FMatrix4 FMatrix4::CreatePerspective(float fovy, float aspect, float zNear, float zFar)
{

	assert(abs(aspect - std::numeric_limits<float>::epsilon()) > 0);

	float const tanHalfFovy = tan(fovy / 2.0f);

	FMatrix4 Result(0);
	Result[0][0] = 1.0f / (aspect * tanHalfFovy);
	Result[1][1] = 1.0f / (tanHalfFovy);
	Result[2][2] = -(zFar + zNear) / (zFar - zNear);
	Result[2][3] = -1.0f;
	Result[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
	return Result;
}

FMatrix4 FMatrix4::CreateOrthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	FMatrix4 Result(1);
	Result[0][0] = 2.0f / (right - left);
	Result[1][1] = 2.0f / (top - bottom);
	Result[2][2] = -2.0f / (zFar - zNear);
	Result[3][0] = -(right + left) / (right - left);
	Result[3][1] = -(top + bottom) / (top - bottom);
	Result[3][2] = -(zFar + zNear) / (zFar - zNear);
	return Result;
}



// --------- OPERATORS -----------------------------------------

FMatrix4 const& operator*(float const& l, FMatrix4 const& r)
{

	return FMatrix4
	(
		r[0] * l,
		r[1] * l,
		r[2] * l,
		r[3] * l
	);
}

FMatrix4 const& operator*(FMatrix4 const& l, float const& r)
{

	return FMatrix4
	(
		l[0] * r,
		l[1] * r,
		l[2] * r,
		l[3] * r
	);
}




FVector4 operator*(FMatrix4 const& l, FVector4 const& r)
{
	FVector4 _v0 = _mm_mul_ps(l[0].mmv, r.mmv);
	FVector4 _v1 = _mm_mul_ps(l[1].mmv, r.mmv);
	FVector4 _v2 = _mm_mul_ps(l[2].mmv, r.mmv);
	FVector4 _v3 = _mm_mul_ps(l[3].mmv, r.mmv);

	return FVector4
	(
		_v0.x + _v0.y + _v0.z + _v0.w,
		_v1.x + _v1.y + _v1.z + _v1.w,
		_v2.x + _v2.y + _v2.z + _v2.w,
		_v3.x + _v3.y + _v3.z + _v3.w
	);
}

FMatrix4& operator*(FMatrix4 const& l, FMatrix4 const& r)
{

	/*
	FMatrix4 m(0);


	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				m[i][k] += l[i][j] * r[j][k];
			}
		}
	}*/

	
	FVector4 const SrcA0 = l[0];
	FVector4 const SrcA1 = l[1];
	FVector4 const SrcA2 = l[2];
	FVector4 const SrcA3 = l[3];

	FVector4 const SrcB0 = r[0];
	FVector4 const SrcB1 = r[1];
	FVector4 const SrcB2 = r[2];
	FVector4 const SrcB3 = r[3];

	FMatrix4 m;
	m[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
	m[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
	m[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
	m[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];

	return m;
}