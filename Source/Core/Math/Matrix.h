#pragma once

#include <Mist_Common\include\UtilityMacros.h>

MIST_NAMESPACE

struct Matrix4x4
{
	float m_Matrix[4][4];
};

inline Matrix4x4 operator*(Matrix4x4 lhs, Matrix4x4 rhs)
{
	Matrix4x4 product;
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			product.m_Matrix[row][col] = lhs.m_Matrix[row][0] * rhs.m_Matrix[0][col] + lhs.m_Matrix[row][1] * rhs.m_Matrix[1][col] + lhs.m_Matrix[row][2] * rhs.m_Matrix[2][col] + lhs.m_Matrix[row][3] * rhs.m_Matrix[3][col];
		}
	}
}

MIST_NAMESPACE_END

