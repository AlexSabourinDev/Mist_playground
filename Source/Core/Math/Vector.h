#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <cstdint>
#include <cmath>

MistNamespace

struct Vec2
{
	float x;
	float y;

	constexpr float& operator[](uint8_t index) { return (&x)[index]; }
};

struct Vec2Int
{
	int x;
	int y;

	constexpr int& operator[](uint8_t index) { return (&x)[index]; }
};

struct Vec3
{
	float x;
	float y;
	float z;

	constexpr float& operator[](uint8_t index) { return (&x)[index]; }
};

constexpr Vec3 operator+(Vec3 lhs, Vec3 rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

constexpr Vec3 operator-(Vec3 lhs, Vec3 rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

constexpr Vec3 operator*(Vec3 lhs, float rhs)
{
	return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
}

constexpr Vec3 operator*(float rhs, Vec3 lhs)
{
	return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
}

constexpr Vec3 operator/(Vec3 lhs, float rhs)
{
	return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs };
}

constexpr Vec3 ComponentMul(Vec3 lhs, Vec3 rhs)
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z };
}

constexpr float Dot(Vec3 lhs, Vec3 rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

inline float Magnitude(Vec3 vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

constexpr Vec3 operator-(Vec3 vec)
{
	return { -vec.x, -vec.y, -vec.z };
}

constexpr Vec3 Cross(Vec3 lhs, Vec3 rhs)
{
	return { lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x };
}

inline Vec3 Normalize(Vec3 vec)
{
	return vec / Magnitude(vec);
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
