#include <Mist_Common\include\UtilityMacros.h>

#include <Math/Serialization.h>

#include <Math/Rect.h>
#include <cJson\cJSON.h>

MIST_NAMESPACE

void Serialize(cJSON* resource, RectInt* rect)
{
	cJSON_AddNumberToObject(resource, "x", rect->x);
	cJSON_AddNumberToObject(resource, "y", rect->y);
	cJSON_AddNumberToObject(resource, "width", rect->width);
	cJSON_AddNumberToObject(resource, "height", rect->height);
}

void Deserialize(cJSON* resource, RectInt* rect)
{
	rect->x = cJSON_GetObjectItem(resource, "x")->valueint;
	rect->y = cJSON_GetObjectItem(resource, "y")->valueint;
	rect->width = cJSON_GetObjectItem(resource, "width")->valueint;
	rect->height = cJSON_GetObjectItem(resource, "height")->valueint;
}

void Serialize(cJSON* resource, Rect* rect)
{
	cJSON_AddNumberToObject(resource, "x", rect->x);
	cJSON_AddNumberToObject(resource, "y", rect->y);
	cJSON_AddNumberToObject(resource, "width", rect->width);
	cJSON_AddNumberToObject(resource, "height", rect->height);
}

void Deserialize(cJSON* resource, Rect* rect)
{
	rect->x = (float)cJSON_GetObjectItem(resource, "x")->valuedouble;
	rect->y = (float)cJSON_GetObjectItem(resource, "y")->valuedouble;
	rect->width = (float)cJSON_GetObjectItem(resource, "width")->valuedouble;
	rect->height = (float)cJSON_GetObjectItem(resource, "height")->valuedouble;
}

void Serialize(cJSON* resource, Vec3* vec)
{
	cJSON_AddNumberToObject(resource, "x", vec->x);
	cJSON_AddNumberToObject(resource, "y", vec->y);
	cJSON_AddNumberToObject(resource, "z", vec->z);
}

void Deserialize(cJSON* resource, Vec3* vec)
{
	vec->x = (float)cJSON_GetObjectItem(resource, "x")->valuedouble;
	vec->y = (float)cJSON_GetObjectItem(resource, "y")->valuedouble;
	vec->z = (float)cJSON_GetObjectItem(resource, "z")->valuedouble;
}

void Serialize(cJSON* resource, Vec4* vec)
{
	cJSON_AddNumberToObject(resource, "x", vec->x);
	cJSON_AddNumberToObject(resource, "y", vec->y);
	cJSON_AddNumberToObject(resource, "z", vec->z);
	cJSON_AddNumberToObject(resource, "w", vec->w);
}

void Deserialize(cJSON* resource, Vec4* vec)
{
	vec->x = (float)cJSON_GetObjectItem(resource, "x")->valuedouble;
	vec->y = (float)cJSON_GetObjectItem(resource, "y")->valuedouble;
	vec->z = (float)cJSON_GetObjectItem(resource, "z")->valuedouble;
	vec->w = (float)cJSON_GetObjectItem(resource, "w")->valuedouble;
}

MIST_NAMESPACE_END