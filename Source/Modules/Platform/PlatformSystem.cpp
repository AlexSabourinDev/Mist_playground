
#include <Mist_Common\include\UtilityMacros.h>

#include <Platform\PlatformSystem.h>
#include <Utility/BitManipulations.h>
#include <Utility/String.h>

#include <Core/Utility/SystemEventHandler.h>

#include <Math\Serialization.h>

#include <SDL.h>

#include <cstdlib>
#include <cstdio>

MIST_NAMESPACE

SystemEventResult TickPlatformSystem(void* system, SystemEventType, SystemEventData);
SystemEventResult ClearScreen(void* system, SystemEventType, SystemEventData);

struct PlatformSystem
{
	WindowConfig m_Config;
	SystemEventDispatch* m_EventSystem;
	SDL_Window* m_Window;
};

void FreeConfig(WindowConfig* config)
{
	Clear(&config->m_WindowName);
}

void Deserialize(cJSON* jsonObject, WindowConfig* config)
{
	Deserialize(cJSON_GetObjectItem(jsonObject, "Rect"), &config->m_Rect);
	
	config->m_WindowFlags = (WindowFlags)0;
	cJSON* windowFlags = cJSON_GetObjectItem(jsonObject, "Flags");
	for (int i = 0; i < cJSON_GetArraySize(windowFlags); i++)
	{
		cJSON* flag = cJSON_GetArrayItem(windowFlags, i);
		if (Equal(flag->valuestring, "Show"))
		{
			// This looks pretty gross ;p
			config->m_WindowFlags = (WindowFlags)((uint32_t)config->m_WindowFlags | (uint32_t)WindowFlags::Show);
		}
		else if (Equal(flag->valuestring, "FullScreen"))
		{
			// This looks pretty gross ;p
			config->m_WindowFlags = (WindowFlags)((uint32_t)config->m_WindowFlags | (uint32_t)WindowFlags::FullScreen);
		}
		// Add additonal flag values here
	}

	Set(&config->m_WindowName, cJSON_GetObjectItem(jsonObject, "Name")->valuestring);
}

Uint32 ConvertWindowFlags(WindowFlags flags)
{
	Uint32 sdlFlags = 0;

	if (IsFlagSet((BitField)flags, (BitField)WindowFlags::Show))
	{
		sdlFlags |= SDL_WINDOW_SHOWN;
	}
	if (IsFlagSet((BitField)flags, (BitField)WindowFlags::FullScreen))
	{
		sdlFlags |= SDL_WINDOW_FULLSCREEN;
	}

	return sdlFlags | SDL_WINDOW_OPENGL;
}

void InitializeOpenGL(SDL_Window* window)
{
	int result = 0;
	// Add the results, if the results are negative the final result will be negative. The success will still be 0
	result += SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	result += SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	result += SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	result += SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_CreateContext(window);

	MIST_ASSERT(result == 0);
}

PlatformSystem* CreatePlatformSystem(SystemAllocator allocator, WindowConfig config)
{
	PlatformSystem* platform = (PlatformSystem*)allocator(sizeof(PlatformSystem));
	platform->m_Config = config;

	if (SDL_WasInit(SDL_INIT_VIDEO))
	{
		WindowConfig* config = &platform->m_Config;
		SDL_Window* window = SDL_CreateWindow(ToCStr(&config->m_WindowName), config->m_Rect.x, config->m_Rect.y,
			config->m_Rect.width, config->m_Rect.height, ConvertWindowFlags(config->m_WindowFlags));
		MIST_ASSERT(window != nullptr);

		platform->m_Window = window;
		InitializeOpenGL(window);
	}

	return platform;
}

void DestroyPlatformSystem(SystemDeallocator deallocator, PlatformSystem* platform)
{
	SDL_DestroyWindow(platform->m_Window);
	FreeConfig(&platform->m_Config);
	deallocator(platform);
}

void ProvideEventSystem(PlatformSystem* system, SystemEventDispatch* eventSystem)
{
	system->m_EventSystem = eventSystem;

	RegisterHandler(eventSystem, SystemEventType::Tick, TickPlatformSystem, system);
	RegisterHandler(eventSystem, SystemEventType::ClearScreen, ClearScreen, system);
}

// Event handlers

SystemEventResult TickPlatformSystem(void* system, SystemEventType, SystemEventData)
{
	PlatformSystem* platform = (PlatformSystem*)system;

	SDL_Event event;
	while (SDL_PollEvent(&event)) 
	{
		switch (event.type)
		{
		case SDL_QUIT:
			DispatchEvent(platform->m_EventSystem, SystemEventType::ShutdownRequest);
			break;
		}
	}

	
	return SystemEventResult::Ok;
}

SystemEventResult ClearScreen(void* system, SystemEventType, SystemEventData)
{
	PlatformSystem* platform = (PlatformSystem*)system;
	SDL_GL_SwapWindow(platform->m_Window);

	return SystemEventResult::Ok;
}


MIST_NAMESPACE_END