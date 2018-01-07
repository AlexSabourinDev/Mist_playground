#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <cstdint>

MistNamespace

struct Vec3
{
	float x;
	float y;
	float z;

	float& operator[](uint8_t index) { return (&x)[index]; }
};

inline Vec3 operator+(Vec3 lhs, Vec3 rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

inline Vec3 operator-(Vec3 lhs, Vec3 rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

inline Vec3 operator*(Vec3 lhs, float rhs)
{
	return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
}

inline Vec3 operator/(Vec3 lhs, float rhs)
{
	return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs };
}

inline Vec3 ComponentMul(Vec3 lhs, Vec3 rhs)
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
}

inline float Dot(Vec3 lhs, Vec3 rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

inline Vec3 operator-(Vec3 vec)
{
	return { -vec.x, -vec.y, -vec.z };
}


struct Vec4
{
	float x;
	float y;
	float z;
	float w;

	float& operator[](uint8_t index) { return (&x)[index]; }
};

inline Vec4 operator+(Vec4 lhs, Vec4 rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
}

inline Vec4 operator-(Vec4 lhs, Vec4 rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
}

inline Vec4 operator*(Vec4 lhs, float rhs)
{
	return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs };
}

inline Vec4 operator/(Vec4 lhs, float rhs)
{
	return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs };
}

inline Vec4 ComponentMul(Vec4 lhs, Vec4 rhs)
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w };
}

inline float Dot(Vec4 lhs, Vec4 rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

MistNamespaceEnd
