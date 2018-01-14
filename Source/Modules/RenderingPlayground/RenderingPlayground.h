#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Core/Systems/System.h>
#include <Core/Systems/SystemEventHandler.h>

#include <Rendering\Renderer.h>

MistNamespace

SystemData InitializeRenderingPlayground(SystemAllocator allocator, const char* dataPath);
void DeinitializeRenderingPlayground(SystemAllocator deallocator, SystemData systemData);

void ProvideRenderingPlaygroundDependencies(SystemData data, SystemEventDispatch* dispatch, Renderer* renderer);

MistNamespaceEnd

