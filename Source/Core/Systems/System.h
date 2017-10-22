#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Systems/CoreSystemTypes.h>

MIST_NAMESPACE

struct System;

using SystemArray = void*;
using SystemData = void*;
using SystemInitialize = void(*)(System* system);
using SystemDeinitialize = void(*)(System* system);
using SystemTick = void(*)(System* system); // TODO: Add a time controller

struct System {
	SystemData m_Data = nullptr;

	SystemInitialize m_Initialize = nullptr;
	SystemDeinitialize m_Deinitialize = nullptr;
	SystemTick m_SystemTick = nullptr;
};

__declspec(dllexport) bool IsValid(System* system);

MIST_NAMESPACE_END
