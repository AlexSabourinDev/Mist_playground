#pragma once

#include <Mist_Common\include\UtilityMacros.h>

MistNamespace

using SystemData = void*;

struct SystemAllocator
{
	using Allocator = void*(*)(void* allocator, size_t size);
	using Deallocator = void(*)(void* allocator, void* data);

	Allocator allocate;
	Deallocator deallocate;

	void* allocatorData;
};

MistNamespaceEnd
