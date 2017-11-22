#pragma once

#include <Mist_Common\include\UtilityMacros.h>

MIST_NAMESPACE

using SystemData = void*;

using SystemAllocator = void*(*)(size_t size);
using SystemDeallocator = void(*)(void* data);

MIST_NAMESPACE_END
