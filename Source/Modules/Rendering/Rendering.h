#pragma once

#include <Mist_Common/include/UtilityMacros.h>

#include <Systems/System.h>

#include <Core/Systems/SystemEventHandler.h>

#include <Rendering/Renderer.h>

MistNamespace

// Interfaces with main, the renderer will select between the correct rendering implementations. 
// This API should not be used extensively as it is mostly a startup and shutdown API.
SystemData InitializeRenderer(SystemAllocator allocator);
void DeinitializeRenderer(SystemDeallocator deallocator, SystemData systemData);

void ProvideRenderingDependencies(SystemData data, SystemEventDispatch* eventHandler);

Renderer* GetRenderer(SystemData system);

MistNamespaceEnd
