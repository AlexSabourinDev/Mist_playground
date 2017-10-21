
#include <Mist_Common\include\UtilityMacros.h>

#include <Platform/Platform.h>
#include <Systems/System.h>

#include <cstdlib>
#include <cstring>

MIST_NAMESPACE

PLATFORM_API void Initialize()
{
	// Does nothing
}

PLATFORM_API void* LoadCoreSystems()
{
	DynamicArray<Pair<System, CoreSystemType>>* systems = (DynamicArray<Pair<System, CoreSystemType>>*)malloc(sizeof(DynamicArray<Pair<System, CoreSystemType>>));
	memset(systems, 0, sizeof(DynamicArray<Pair<System, CoreSystemType>>));

	System platformSystem;
	PushBack(systems, Create(platformSystem, CoreSystemType::Platform));

	return systems;
}

PLATFORM_API void ReleaseSystemArray(void* data)
{
	DynamicArray<Pair<System, CoreSystemType>>* systems = (DynamicArray<Pair<System, CoreSystemType>>*)data;
	Clear(systems);
	free(systems);
}

PLATFORM_API void Deinitialize()
{
	// Does nothing
}

MIST_NAMESPACE_END