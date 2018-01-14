#pragma once

#include <Mist_common\include\UtilityMacros.h>

#include <Math/Vector.h>

#include <cmath>

MistNamespace

constexpr Vec3 Evaluate(Vec3 p0, Vec3 p1, Vec3 p2, Vec3 p3, float t)
{
	return pow(1.0f - t, 3.0f) * p0 + 3.0f * pow(1.0f - t, 2.0f) * t * p1 + 3.0f*(1.0f - t) * pow(t, 2.0f) * p2 + pow(t, 3.0f) * p3;
}

constexpr Vec3 Lerp(Vec3 p0, Vec3 p1, float t)
{
	return (1.0f - t) * p0 + t * p1;
}

MistNamespaceEnd

