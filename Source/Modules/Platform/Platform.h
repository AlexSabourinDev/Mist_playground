#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Platform/PlatformAPIh.h>

MIST_NAMESPACE

extern "C"
{
	struct System;
	struct CoreSystemCollection;

	PLATFORM_API void Initialize(System** systems, size_t* systemCount);
	PLATFORM_API void LinkDependencies(CoreSystemCollection* systemCollection);
	PLATFORM_API void Deinitialize(CoreSystemCollection* systemCollection);

}

MIST_NAMESPACE_END
