#pragma once

#include <Mist_Common\include\UtilityMacros.h>

MIST_NAMESPACE

template< typename RectType >
struct RectTemplate
{
	RectType x;
	RectType y;
	RectType width;
	RectType height;
};

using Rect = RectTemplate<float>;
using RectInt = RectTemplate<int>;

MIST_NAMESPACE_END

