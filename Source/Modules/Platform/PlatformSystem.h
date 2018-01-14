#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Systems\System.h>

#include <Core/Systems/SystemEventHandler.h>
#include <Utility\String.h>

#include <Math\Rect.h>

#include <cJson\cJSON.h>

#include <cstdint>

MistNamespace

enum class WindowFlags : uint32_t
{
	Show = 1 << 0,
	FullScreen = 1 << 1
};

struct WindowConfig
{
	String windowName;
	RectInt rect;
	WindowFlags windowFlags;
};

struct PlatformSystem;

PlatformSystem* CreatePlatformSystem(SystemAllocator allocator, WindowConfig config);
void DestroyPlatformSystem(SystemAllocator allocator, PlatformSystem* platformSystem);

void ProvideEventSystem(PlatformSystem* system, SystemEventDispatch* handler);

// Deserializes the window configuration
void Deserialize(cJSON* jsonObject, WindowConfig* config);

MistNamespaceEnd
