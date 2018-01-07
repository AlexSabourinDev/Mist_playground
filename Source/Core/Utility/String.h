#pragma once

#include <Mist_Common\include\UtilityMacros.h>

MistNamespace

// -String-
struct String
{
	char* stringBuffer = nullptr;
	size_t capacity = 0;
};

// -API-
 String Create(const char* data);

void Append(String* string, const char* appendage);
void Append(String* left, String* right);

void Set(String* string, const char* data);

void Clear(String* string);

bool IsEmpty(const String* string);

bool Equal(const String* left, const String* right);
bool Equal(const String* left, const char* right);
bool Equal(const char* left, const char* right);

void Replace(String* string, const String* target, const String* replaceValue);
void Replace(String* string, const char* target, const char* replaceValue);

bool Contains(String* target, const String* findTarget);
bool Contains(String* target, const char* findTarget);

void Reserve(String* string, size_t size);

const char* ToCStr(String* string);


MistNamespaceEnd
