#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Systems/System.h>
#include <Systems/CoreSystemTypes.h>

MIST_NAMESPACE

struct CoreSystemCollection {
	System m_Systems[(int)CoreSystemType::Max];
};


MIST_NAMESPACE_END