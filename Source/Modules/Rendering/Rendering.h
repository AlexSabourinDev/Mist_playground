#pragma once

#include <Mist_Common/include/UtilityMacros.h>

#include <Systems/System.h>

#include <Utility/SystemEventHandler.h>

MIST_NAMESPACE

extern "C"
{
	SystemData InitializeRenderer(SystemAllocator allocator);
	void DeinitializeRenderer(SystemDeallocator deallocator, SystemData systemData);

	void ProvideRenderingDependencies(SystemData data, SystemEventDispatch* eventHandler);
}

MIST_NAMESPACE_END

