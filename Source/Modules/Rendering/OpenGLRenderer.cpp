#include <Mist_Common\include\UtilityMacros.h>

#include <Utility\SystemEventHandler.h>

#include <Systems/System.h>

#include <cstdlib>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>

MIST_NAMESPACE

struct RenderingSystem
{
	SystemEventHandlers* m_EventSystem;
};

SystemEventResult TickRenderer(System* system, SystemEventType, SystemData)
{
	RenderingSystem* renderingSystem = (RenderingSystem*)system->m_Data;

	glClearColor(0.8f, 0.6f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	DispatchEvent(renderingSystem->m_EventSystem, SystemEventType::ClearScreen);
	return SystemEventResult::Ok;
}

void InitializeRenderer(System* system, SystemEventHandlers* eventSystem)
{
	RegisterHandler(eventSystem, SystemEventType::Tick, TickRenderer, system);

	RenderingSystem* renderingSystem = (RenderingSystem*)system->m_Data;
	renderingSystem->m_EventSystem = eventSystem;

#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif
}

void DeinitializeRenderer(System* system)
{
	free(system->m_Data);
}

System CreateRenderer()
{
	System rendererSystem;
	rendererSystem.m_Initialize = InitializeRenderer;
	rendererSystem.m_Deinitialize = DeinitializeRenderer;
	rendererSystem.m_Data = malloc(sizeof(RenderingSystem));

	return rendererSystem;
}

MIST_NAMESPACE_END

