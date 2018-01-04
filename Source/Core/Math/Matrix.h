#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Core/Math/Vector.h>

#include <stdint.h>

MistNamespace

struct alignas(16) Mat4
{
	struct alignas(16) MatLine
	{
		float col[4];
		float& operator[](uint8_t index) { return col[index]; }
	};

	MatLine data[4];
	MatLine& operator[](uint8_t index) { return { data[index] }; }
};

inline Mat4 operator*(Mat4 lhs, Mat4 rhs)
{
	Mat4 product;
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			product[row][col] = lhs[row][0] * rhs[0][col] + lhs[row][1] * rhs[1][col] + lhs[row][2] * rhs[2][col] + lhs[row][3] * rhs[3][col];
		}
	}
	return product;
}

inline Vec4 operator*(Mat4 lhs, Vec4 rhs)
{
	Vec4 product;
	for (int row = 0; row < 4; row++)
	{
		product[row] = lhs[row][0] * rhs.x + lhs[row][1] * rhs.y + lhs[row][2] * rhs.z + lhs[row][3] * rhs.w;
	}
	return product;
}

inline Mat4 Identity()
{
	return	{ { {1.0f, 0.0f, 0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f, 0.0f},
				{0.0f, 0.0f, 1.0f, 0.0f},
				{0.0f, 0.0f, 0.0f, 1.0f} } };
}

MistNamespaceEnd

