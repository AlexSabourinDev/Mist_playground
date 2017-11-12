
#include <Mist_Common\include\UtilityMacros.h>

#include <Rendering\Rendering.h>

#include <Utility/DynamicArray.h>
#include <Utility/Pair.h>

#include <cstring>

MIST_NAMESPACE

// Forward declare the rendering creation that will be used by other renderers.
System CreateRenderer();

__declspec(dllexport) SystemArray Initialize(int argc, char *argv[])
{

	DynamicArray<Pair<System, CoreSystemType>>* systems = (DynamicArray<Pair<System, CoreSystemType>>*)malloc(sizeof(DynamicArray<Pair<System, CoreSystemType>>));
	memset(systems, 0, sizeof(DynamicArray<Pair<System, CoreSystemType>>));

	PushBack(systems, Create(CreateRenderer(), CoreSystemType::Rendering));

	return systems;
}


__declspec(dllexport) void ReleaseSystemArray(SystemArray data)
{
	DynamicArray<Pair<System, CoreSystemType>>* systems = (DynamicArray<Pair<System, CoreSystemType>>*)data;

	Clear(systems);
	free(systems);
}


__declspec(dllexport) void Deinitialize()
{

}

MIST_NAMESPACE_END