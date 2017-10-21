#include <Mist_Common\include\UtilityMacros.h>

#include <Utility/String.h>

#include <cstring>
#include <cstdlib>
#include <cinttypes>

MIST_NAMESPACE

__declspec(dllexport) String Create(const char* data)
{
	String str;
	Set(&str, data);
	return str;
}

__declspec(dllexport) void Append(String* string, const char* appendage)
{
	// Create a new buffer of this string's size + the other string's size
	size_t newSize = string->m_StringBuffer != nullptr ? strlen(string->m_StringBuffer) + strlen(appendage) : strlen(appendage);
	if (newSize > string->m_Capacity)
	{
		Reserve(string, newSize);
	}

	strcat(string->m_StringBuffer, appendage);
}
__declspec(dllexport) void Append(String* left, String* right)
{
	Append(left, right->m_StringBuffer);
}

__declspec(dllexport) void Set(String* string, const char* data)
{
	size_t requestedSize = strlen(data);
	if (string->m_StringBuffer == nullptr || strlen(string->m_StringBuffer) < requestedSize)
	{
		Reserve(string, requestedSize);
	}

	strcpy(string->m_StringBuffer, data);
}

__declspec(dllexport) void Clear(String* string)
{
	if (string->m_StringBuffer == nullptr)
	{
		return;
	}

#if MIST_DEBUG
	memset(string->m_StringBuffer, 0, strlen(string->m_StringBuffer));
#endif

	free(string->m_StringBuffer);
	string->m_Capacity = 0;
	string->m_StringBuffer = nullptr;
}

__declspec(dllexport) bool IsEmpty(const String* string)
{
	return string->m_StringBuffer == nullptr || strlen(string->m_StringBuffer) == 0;
}

__declspec(dllexport) bool Equal(const String* left, const String* right)
{
	return Equal(left, right->m_StringBuffer);
}
__declspec(dllexport) bool Equal(const String* left, const char* right)
{
	return left->m_StringBuffer != nullptr && strcmp(left->m_StringBuffer, right) == 0;
}

__declspec(dllexport) void Replace(String* string, const String* target, const String* replaceValue)
{
	Replace(string, target->m_StringBuffer, replaceValue->m_StringBuffer);
}
__declspec(dllexport) void Replace(String* string, const char* target, const char* replaceValue)
{
	if (string->m_StringBuffer == nullptr)
	{
		return;
	}

	// Find all occurences of the target and resize accordingly
	char* substringPos = nullptr;
	while ((substringPos = strstr(string->m_StringBuffer, target)) != nullptr)
	{
		size_t targetSize = strlen(target);
		size_t replaceSize = strlen(replaceValue);
		size_t stringSize = strlen(string->m_StringBuffer);
		// Possible overflow here, but if a string is over 2 billion characters, that's a problem
		int sizeDifference = (int)strlen(replaceValue) - (int)targetSize;
		if ((int)stringSize + sizeDifference > (int)string->m_Capacity)
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
	return Contains(target, findTarget->m_StringBuffer);
}
__declspec(dllexport) bool Contains(String* target, const char* findTarget)
{
	return target->m_StringBuffer != nullptr && strstr(target->m_StringBuffer, findTarget) != nullptr;
}

__declspec(dllexport) void Reserve(String* string, size_t size)
{
	if (string->m_Capacity >= size || size == 0)
	{
		return;
	}

	// Always add 1 to the requested size in order to have a zero terminated string
	char* result = (char*)realloc(string->m_StringBuffer, size + 1);

	if (result == nullptr)
	{
		// Realloc failed to reallocate the block of memory
		MIST_ASSERT(false);
	}
	else
	{
		size_t previousCapacity = string->m_Capacity;
		memset(result + previousCapacity, 0, size - previousCapacity + 1);
		string->m_StringBuffer = result;
		string->m_Capacity = size;
	}
}

__declspec(dllexport) const char* ToCStr(String* string)
{
	return string->m_StringBuffer != nullptr ? string->m_StringBuffer : "";
}

MIST_NAMESPACE_END

