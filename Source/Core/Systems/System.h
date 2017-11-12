#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Systems/CoreSystemTypes.h>

MIST_NAMESPACE

struct System;
struct SystemEventHandlers;

using SystemArray = void*;
using SystemData = void*;
using SystemInitialize = void(*)(System* system, SystemEventHandlers* eventSystem);
using SystemDeinitialize = void(*)(System* system);

struct System {
	SystemData m_Data = nullptr;

	SystemInitialize m_Initialize = nullptr;
	SystemDeinitialize m_Deinitialize = nullptr;
};

__declspec(dllexport) bool IsValid(System* system);

MIST_NAMESPACE_END
