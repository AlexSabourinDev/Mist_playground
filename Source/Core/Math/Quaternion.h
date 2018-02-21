#pragma once

#include <Mist_common\include\UtilityMacros.h>

#include <Core/Math/Vector.h>
#include <Core/Math/Matrix.h>

#include <cmath>

MistNamespace

struct Quaternion
{
	float x;
	float y;
	float z;
	float w;
};

inline Quaternion AxisAngle(Vec3 axis, float angle)
{
	float s = sin(angle / 2.0f);
	return { axis.x * s, axis.y * s, axis.z * s, cos(angle / 2.0f) };
}

inline Quaternion Euler(Vec3 euler)
{
	float c1 = cos(euler.y / 2.0f);
	float c2 = cos(euler.x / 2.0f);
	float c3 = cos(euler.z / 2.0f);

	float s1 = sin(euler.y / 2.0f);
	float s2 = sin(euler.x / 2.0f);
	float s3 = sin(euler.z / 2.0f);

	Quaternion quat;
	quat.w = c1*c2*c3 - s1*s2*s3;
	quat.x = s1*s2*c3 + c1*c2*s3;
	quat.y = s1*c2*c3 + c1*s2*s3;
	quat.z = c1*s2*c3 - s1*c2*s3;

	return quat;
}

inline Quaternion Conjugate(Quaternion quat)
{
	return { -quat.x, -quat.y, -quat.z, quat.w };
}

inline float Norm(Quaternion quat)
{
	return sqrt(quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w);
}

inline Quaternion Inverse(Quaternion quat)
{
	Quaternion conj = Conjugate(quat);
	float div = 1.0f / quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w;
	return { conj.x * div, conj.y * div, conj.z * div, conj.w * div };
}

inline Mat4 ToMatrix(Quaternion quat)
{
	return	{ { { 1.0f - 2.0f*quat.y*quat.y - 2.0f*quat.z*quat.z, 2.0f*quat.x*quat.y - 2.0f*quat.z*quat.w, 2.0f*quat.x*quat.z + 2.0f*quat.y*quat.w, 0.0f },
				{ 2.0f*quat.x*quat.y + 2.0f*quat.z*quat.w, 1.0f - 2.0f*quat.x*quat.x - 2.0f*quat.z*quat.z, 2.0f*quat.y*quat.z - 2.0f*quat.x*quat.w, 0.0f },
				{ 2.0f*quat.x*quat.z - 2.0f*quat.y*quat.w, 2.0f*quat.y*quat.z + 2.0f*quat.x*quat.w, 1.0f - 2.0f*quat.x*quat.x - 2.0f*quat.y*quat.y, 0.0f },
				{ 0.0f, 0.0f, 0.0f, 1.0f } } };
}

inline Quaternion operator*(Quaternion lhs, Quaternion rhs)
{
	Quaternion quat;
	quat.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
	quat.x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
	quat.y = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x;
	quat.z = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w;
	return quat;
}

inline Vec3 operator*(Quaternion lhs, Vec3 rhs)
{
	Vec3 q = { lhs.x, lhs.y, lhs.z };
	Vec3 t = 2.0f * Cross(q, rhs);
	return rhs + lhs.w * t + Cross(q, t);
}

inline Quaternion IdentityQuat()
{
	return { 0.0f, 0.0f, 0.0f, 1.0f };
}

MistNamespaceEnd
