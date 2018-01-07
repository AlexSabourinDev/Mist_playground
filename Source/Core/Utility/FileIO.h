#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Utility\String.h>

#include <cstdint>

MistNamespace

String ReadFile(const char* file);

// Binary operations
void ReadFile(const char* file, uint8_t** data, size_t* dataSize);
void WriteFile(const char* file, uint8_t* data, size_t dataSize);

MistNamespaceEnd
