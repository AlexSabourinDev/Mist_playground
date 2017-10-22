#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Math/Rect.h>

struct cJSON;

MIST_NAMESPACE

__declspec(dllexport) void Serialize(cJSON* resource, RectInt* rect);
__declspec(dllexport) void Deserialize(cJSON* resource, RectInt* rect);

__declspec(dllexport) void Serialize(cJSON* resource, Rect* rect);
__declspec(dllexport) void Deserialize(cJSON* resource, Rect* rect);


MIST_NAMESPACE_END

