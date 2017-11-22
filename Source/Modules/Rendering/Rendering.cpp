
#include <Mist_Common\include\UtilityMacros.h>

#include <Rendering\Rendering.h>

#include <cstring>

MIST_NAMESPACE

// Forward declare the rendering creation that will be used by other renderers.
struct RenderingSystem;
RenderingSystem* CreateRenderer(SystemAllocator allocator);

void ProvideEventDispatchToRenderer(RenderingSystem* renderingSystem, SystemEventDispatch* eventSystem);

SystemData InitializeRenderer(SystemAllocator allocator)
{
	return CreateRenderer(allocator);
}

void DeinitializeRenderer(SystemDeallocator deallocator, SystemData systemData)
{
	deallocator(systemData);
}

void ProvideRenderingDependencies(SystemData data, SystemEventDispatch* eventHandler)
{
	ProvideEventDispatchToRenderer((RenderingSystem*)data, eventHandler);
}

MIST_NAMESPACE_END