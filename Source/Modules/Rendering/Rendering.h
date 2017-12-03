#pragma once

#include <Mist_Common/include/UtilityMacros.h>

#include <Systems/System.h>

#include <Utility/SystemEventHandler.h>

#include <Rendering/Renderer.h>

MIST_NAMESPACE

SystemData InitializeRenderer(SystemAllocator allocator);
void DeinitializeRenderer(SystemDeallocator deallocator, SystemData systemData);

void ProvideRenderingDependencies(SystemData data, SystemEventDispatch* eventHandler);

Renderer* GetRenderer(SystemData system);

MIST_NAMESPACE_END
