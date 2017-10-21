#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Systems/CoreSystemCollection.h>
#include <Platform/PlatformAPI.h>

#include <Utility/DynamicArray.h>
#include <Utility/Pair.h>

MIST_NAMESPACE

extern "C"
{
	struct System;
	struct CoreSystemCollection;

	PLATFORM_API void Initialize();
	PLATFORM_API void* LoadCoreSystems();
	PLATFORM_API void ReleaseSystemArray(void* systems);
	PLATFORM_API void Deinitialize();

}

MIST_NAMESPACE_END
