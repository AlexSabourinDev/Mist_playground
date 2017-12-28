#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Systems/System.h>

#include <Core/Systems/SystemEventHandler.h>

MistNamespace

SystemData InitializePlatform(SystemAllocator allocator, const char* dataPath);
void DeinitializePlatform(SystemDeallocator deallocator, SystemData data);

// The platform module requries a system event handler in order to communicate to other systems
void ProvidePlatformDependencies(SystemData data, SystemEventDispatch* eventHandler);

MistNamespaceEnd
