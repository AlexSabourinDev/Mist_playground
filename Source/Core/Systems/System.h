#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Systems/CoreSystemTypes.h>

MIST_NAMESPACE

using SystemData = void*;
struct System {
	SystemData m_Data = nullptr;
};

MIST_NAMESPACE_END
