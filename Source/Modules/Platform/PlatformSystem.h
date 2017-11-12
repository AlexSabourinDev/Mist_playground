#pragma once

#include <Mist_Common\include\UtilityMacros.h>
#include <Systems\System.h>
#include <Utility\String.h>
#include <Math\Rect.h>

#include <cJson\cJSON.h>

#include <cstdint>

MIST_NAMESPACE

enum class WindowFlags : uint32_t
{
	Show = 1 << 0,
	FullScreen = 1 << 1
};

struct WindowConfig
{
	String m_WindowName;
	RectInt m_Rect;
	WindowFlags m_WindowFlags;
};

System CreatePlatformSystem(WindowConfig config);
void Deserialize(cJSON* jsonObject, WindowConfig* config);

MIST_NAMESPACE_END
