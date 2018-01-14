
#include <Mist_Common\include\UtilityMacros.h>

#include <Rendering\Rendering.h>
#include <Rendering/Renderer.h>

#include <cstring>

MistNamespace

// Forward declare the rendering creation that will be used by other renderers.
Renderer* CreateRenderer(SystemAllocator allocator);
void DestroyRenderer(SystemAllocator deallocator, Renderer* renderer);

void ProvideEventDispatchToRenderer(Renderer* renderingSystem, SystemEventDispatch* eventSystem);

SystemData InitializeRenderer(SystemAllocator allocator)
{
	return CreateRenderer(allocator);
}

void DeinitializeRenderer(SystemAllocator deallocator, SystemData systemData)
{
	DestroyRenderer(deallocator, (Renderer*)systemData);
}

void ProvideRenderingDependencies(SystemData data, SystemEventDispatch* eventHandler)
{
	ProvideEventDispatchToRenderer((Renderer*)data, eventHandler);
}

Renderer* GetRenderer(SystemData system)
{
	return (Renderer*)system;
}

MistNamespaceEnd