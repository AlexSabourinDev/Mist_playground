#pragma once

#include <Mist_Common\include\UtilityMacros.h>

MistNamespace

using SystemData = void*;

using SystemAllocator = void*(*)(size_t size);
using SystemDeallocator = void(*)(void* data);

MistNamespaceEnd
