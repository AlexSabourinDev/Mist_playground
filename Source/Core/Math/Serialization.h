#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Math/Rect.h>
#include <Math/Vector.h>

struct cJSON;

MistNamespace

void Serialize(cJSON* resource, RectInt* rect);
void Deserialize(cJSON* resource, RectInt* rect);

void Serialize(cJSON* resource, Rect* rect);
void Deserialize(cJSON* resource, Rect* rect);

void Serialize(cJSON* resource, Vec3* vec);
void Deserialize(cJSON* resource, Vec3* vec);

void Serialize(cJSON* resource, Vec4* vec);
void Deserialize(cJSON* resource, Vec4* vec);

MistNamespaceEnd

