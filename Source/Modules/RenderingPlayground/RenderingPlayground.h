#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Core/Systems/System.h>
#include <Core/Utility/SystemEventHandler.h>

#include <Rendering\Renderer.h>

MIST_NAMESPACE

SystemData InitializeRenderingPlayground(SystemAllocator allocator);
void DeinitializeRenderingPlayground(SystemDeallocator deallocator, SystemData systemData);

void ProvideRenderingPlaygroundDependencies(SystemData data, Renderer* renderer);

MIST_NAMESPACE_END

