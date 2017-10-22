#include <Mist_Common\include\UtilityMacros.h>

#include <Math/Serialization.h>

#include <Math/Rect.h>
#include <cJson\cJSON.h>

MIST_NAMESPACE

__declspec(dllexport) void Serialize(cJSON* resource, RectInt* rect)
{
	cJSON_AddNumberToObject(resource, "x", rect->x);
	cJSON_AddNumberToObject(resource, "y", rect->y);
	cJSON_AddNumberToObject(resource, "width", rect->width);
	cJSON_AddNumberToObject(resource, "height", rect->height);
}

__declspec(dllexport) void Deserialize(cJSON* resource, RectInt* rect)
{
	rect->x = cJSON_GetObjectItem(resource, "x")->valueint;
	rect->y = cJSON_GetObjectItem(resource, "y")->valueint;
	rect->width = cJSON_GetObjectItem(resource, "width")->valueint;
	rect->height = cJSON_GetObjectItem(resource, "height")->valueint;
}

__declspec(dllexport) void Serialize(cJSON* resource, Rect* rect)
{
	cJSON_AddNumberToObject(resource, "x", rect->x);
	cJSON_AddNumberToObject(resource, "y", rect->y);
	cJSON_AddNumberToObject(resource, "width", rect->width);
	cJSON_AddNumberToObject(resource, "height", rect->height);
}

__declspec(dllexport) void Deserialize(cJSON* resource, Rect* rect)
{
	rect->x = (float)cJSON_GetObjectItem(resource, "x")->valuedouble;
	rect->y = (float)cJSON_GetObjectItem(resource, "y")->valuedouble;
	rect->width = (float)cJSON_GetObjectItem(resource, "width")->valuedouble;
	rect->height = (float)cJSON_GetObjectItem(resource, "height")->valuedouble;
}

MIST_NAMESPACE_END