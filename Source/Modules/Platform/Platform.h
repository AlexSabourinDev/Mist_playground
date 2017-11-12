#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Systems/CoreSystemCollection.h>
#include <Systems/System.h>
#include <Platform/PlatformAPI.h>

#include <Utility/DynamicArray.h>
#include <Utility/Pair.h>

MIST_NAMESPACE

extern "C"
{
	PLATFORM_API SystemArray Initialize(int argc, char *argv[]);
	PLATFORM_API void ReleaseSystemArray(SystemArray systems);
	PLATFORM_API void Deinitialize();
}

MIST_NAMESPACE_END
