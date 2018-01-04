#include <Mist_Common\include\UtilityMacros.h>

#include <Utility/String.h>

#include <cstring>
#include <cstdlib>
#include <cinttypes>

MistNamespace

__declspec(dllexport) String Create(const char* data)
{
	String str;
	Set(&str, data);
	return str;
}

__declspec(dllexport) void Append(String* string, const char* appendage)
{
	// Create a new buffer of this string's size + the other string's size
	size_t newSize = string->stringBuffer != nullptr ? strlen(string->stringBuffer) + strlen(appendage) : strlen(appendage);
	if (newSize > string->capacity)
	{
		Reserve(string, newSize);
	}

	strcat(string->stringBuffer, appendage);
}
__declspec(dllexport) void Append(String* left, String* right)
{
	Append(left, right->stringBuffer);
}

__declspec(dllexport) void Set(String* string, const char* data)
{
	size_t requestedSize = strlen(data);
	if (string->stringBuffer == nullptr || strlen(string->stringBuffer) < requestedSize)
	{
		Reserve(string, requestedSize);
	}

	strcpy(string->stringBuffer, data);
}

__declspec(dllexport) void Clear(String* string)
{
	if (string->stringBuffer == nullptr)
	{
		return;
	}

#if MIST_DEBUG
	memset(string->stringBuffer, 0, strlen(string->stringBuffer));
#endif

	free(string->stringBuffer);
	string->capacity = 0;
	string->stringBuffer = nullptr;
}

__declspec(dllexport) bool IsEmpty(const String* string)
{
	return string->stringBuffer == nullptr || strlen(string->stringBuffer) == 0;
}

__declspec(dllexport) bool Equal(const String* left, const String* right)
{
	return Equal(left, right->stringBuffer);
}
__declspec(dllexport) bool Equal(const String* left, const char* right)
{
	return left->stringBuffer != nullptr && strcmp(left->stringBuffer, right) == 0;
}

__declspec(dllexport) bool Equal(const char* left, const char* right)
{
	return strcmp(left, right) == 0;
}

__declspec(dllexport) void Replace(String* string, const String* target, const String* replaceValue)
{
	Replace(string, target->stringBuffer, replaceValue->stringBuffer);
}
__declspec(dllexport) void Replace(String* string, const char* target, const char* replaceValue)
{
	if (string->stringBuffer == nullptr)
	{
		return;
	}

	// Find all occurences of the target and resize accordingly
	char* substringPos = nullptr;
	while ((substringPos = strstr(string->stringBuffer, target)) != nullptr)
	{
		size_t targetSize = strlen(target);
		size_t replaceSize = strlen(replaceValue);
		size_t stringSize = strlen(string->stringBuffer);
		// Possible overflow here, but if a string is over 2 billion characters, that's a problem
		int sizeDifference = (int)strlen(replaceValue) - (int)targetSize;
		if ((int)stringSize + sizeDifference > (int)string->capacity)
		{
			Reserve(string, stringSize + (size_t)sizeDifference);
		}

		size_t appendageLength = strlen(substringPos + targetSize);
		if (appendageLength > 0)
		{
			strncpy(substringPos + replaceSize, substringPos + targetSize, appendageLength);
		}

		if (replaceSize > 0)
		{
			strncpy(substringPos, replaceValue, replaceSize);
		}

		char* memsetLocation = substringPos + replaceSize + appendageLength;
		size_t memSetSize = strlen(memsetLocation);
		memset(memsetLocation, 0, memSetSize);
	}
}

__declspec(dllexport) bool Contains(String* target, const String* findTarget)
{
	return Contains(target, findTarget->stringBuffer);
}
__declspec(dllexport) bool Contains(String* target, const char* findTarget)
{
	return target->stringBuffer != nullptr && strstr(target->stringBuffer, findTarget) != nullptr;
}

__declspec(dllexport) void Reserve(String* string, size_t size)
{
	if (string->capacity >= size || size == 0)
	{
		return;
	}

	// Always add 1 to the requested size in order to have a zero terminated string
	char* result = (char*)realloc(string->stringBuffer, size + 1);

	if (result == nullptr)
	{
		// Realloc failed to reallocate the block of memory
		MistAssert(false);
	}
	else
	{
		size_t previousCapacity = string->capacity;
		memset(result + previousCapacity, 0, size - previousCapacity + 1);
		string->stringBuffer = result;
		string->capacity = size;
	}
}

__declspec(dllexport) const char* ToCStr(String* string)
{
	return string->stringBuffer != nullptr ? string->stringBuffer : "";
}

MistNamespaceEnd

