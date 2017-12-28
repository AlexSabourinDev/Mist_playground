#pragma once

#include <Mist_Common\include\UtilityMacros.h>

MistNamespace

// -String-
struct String
{
	char* m_StringBuffer = nullptr;
	size_t m_Capacity = 0;
};

// -API-
__declspec(dllexport) String Create(const char* data);

__declspec(dllexport) void Append(String* string, const char* appendage);
__declspec(dllexport) void Append(String* left, String* right);

__declspec(dllexport) void Set(String* string, const char* data);

__declspec(dllexport) void Clear(String* string);

__declspec(dllexport) bool IsEmpty(const String* string);

__declspec(dllexport) bool Equal(const String* left, const String* right);
__declspec(dllexport) bool Equal(const String* left, const char* right);
__declspec(dllexport) bool Equal(const char* left, const char* right);

__declspec(dllexport) void Replace(String* string, const String* target, const String* replaceValue);
__declspec(dllexport) void Replace(String* string, const char* target, const char* replaceValue);

__declspec(dllexport) bool Contains(String* target, const String* findTarget);
__declspec(dllexport) bool Contains(String* target, const char* findTarget);

__declspec(dllexport) void Reserve(String* string, size_t size);

__declspec(dllexport) const char* ToCStr(String* string);


MistNamespaceEnd
