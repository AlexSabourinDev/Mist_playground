#include <Mist_Common\include\UtilityMacros.h>

#include <Utility\SystemEventHandler.h>

#include <Systems/System.h>

#include <cstdlib>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

MIST_NAMESPACE

struct RenderingSystem
{
	SystemEventDispatch* m_EventSystem;
};

SystemEventResult TickRenderer(void* system, SystemEventType, SystemData)
{
	RenderingSystem* renderingSystem = (RenderingSystem*)system;

	glClearColor(0.8f, 0.6f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	DispatchEvent(renderingSystem->m_EventSystem, SystemEventType::ClearScreen);
	return SystemEventResult::Ok;
}

void ProvideEventDispatchToRenderer(RenderingSystem* renderingSystem, SystemEventDispatch* eventSystem)
{
	RegisterHandler(eventSystem, SystemEventType::Tick, TickRenderer, renderingSystem);

	renderingSystem->m_EventSystem = eventSystem;

#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif
}

RenderingSystem* CreateRenderer(SystemAllocator allocator)
{
	return (RenderingSystem*)allocator(sizeof(RenderingSystem));
}

MIST_NAMESPACE_END

