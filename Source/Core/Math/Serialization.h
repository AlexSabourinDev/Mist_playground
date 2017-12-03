#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Math/Rect.h>
#include <Math/Vector.h>

struct cJSON;

MIST_NAMESPACE

void Serialize(cJSON* resource, RectInt* rect);
void Deserialize(cJSON* resource, RectInt* rect);

void Serialize(cJSON* resource, Rect* rect);
void Deserialize(cJSON* resource, Rect* rect);

void Serialize(cJSON* resource, Vec3* vec);
void Deserialize(cJSON* resource, Vec3* vec);

MIST_NAMESPACE_END

