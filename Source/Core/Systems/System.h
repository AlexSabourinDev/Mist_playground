#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Systems/CoreSystemTypes.h>

MIST_NAMESPACE

struct System;

using SystemData = void*;
using SystemInitialize = void(*)(System* system);
using SystemDeinitialize = void(*)(System* system);

struct System {
	SystemData m_Data = nullptr;

	SystemInitialize m_Initialize;
	SystemDeinitialize m_Deinitialize;
};

MIST_NAMESPACE_END
