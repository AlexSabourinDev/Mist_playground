
#include <Mist_Common\include\UtilityMacros.h>

#include <Platform\PlatformSystem.h>
#include <Utility/BitManipulations.h>
#include <Utility/String.h>

#include <Core/Systems/SystemEventHandler.h>

#include <Math\Serialization.h>

#include <SDL.h>

#include <cstdlib>
#include <cstdio>

MistNamespace

SystemEventResult TickPlatformSystem(void* system, SystemEventType, SystemEventData);
SystemEventResult ClearScreen(void* system, SystemEventType, SystemEventData);

struct PlatformSystem
{
	WindowConfig config;
	SystemEventDispatch* eventSystem;
	SDL_Window* window;
};

void FreeConfig(WindowConfig* config)
{
	Clear(&config->windowName);
}

void Deserialize(cJSON* jsonObject, WindowConfig* config)
{
	Deserialize(cJSON_GetObjectItem(jsonObject, "Rect"), &config->rect);
	
	config->windowFlags = (WindowFlags)0;
	cJSON* windowFlags = cJSON_GetObjectItem(jsonObject, "Flags");
	for (int i = 0; i < cJSON_GetArraySize(windowFlags); i++)
	{
		cJSON* flag = cJSON_GetArrayItem(windowFlags, i);
		if (Equal(flag->valuestring, "Show"))
		{
			// This looks pretty gross ;p
			config->windowFlags = (WindowFlags)((uint32_t)config->windowFlags | (uint32_t)WindowFlags::Show);
		}
		else if (Equal(flag->valuestring, "FullScreen"))
		{
			// This looks pretty gross ;p
			config->windowFlags = (WindowFlags)((uint32_t)config->windowFlags | (uint32_t)WindowFlags::FullScreen);
		}
		// Add additonal flag values here
	}

	Set(&config->windowName, cJSON_GetObjectItem(jsonObject, "Name")->valuestring);
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

	MistAssert(result == 0);
}

PlatformSystem* CreatePlatformSystem(SystemAllocator allocator, WindowConfig config)
{
	PlatformSystem* platform = (PlatformSystem*)allocator.allocate(allocator.allocatorData, sizeof(PlatformSystem));
	platform->config = config;

	if (SDL_WasInit(SDL_INIT_VIDEO))
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
		SDL_GL_SetSwapInterval(0);

		WindowConfig* config = &platform->config;
		SDL_Window* window = SDL_CreateWindow(ToCStr(&config->windowName), config->rect.x, config->rect.y,
			config->rect.width, config->rect.height, ConvertWindowFlags(config->windowFlags));
		MistAssert(window != nullptr);

		platform->window = window;
		InitializeOpenGL(window);
	}

	return platform;
}

void DestroyPlatformSystem(SystemAllocator allocator, PlatformSystem* platform)
{
	SDL_DestroyWindow(platform->window);
	FreeConfig(&platform->config);
	allocator.deallocate(allocator.allocatorData, platform);
}

void ProvideEventSystem(PlatformSystem* system, SystemEventDispatch* eventSystem)
{
	system->eventSystem = eventSystem;

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
			DispatchEvent(platform->eventSystem, SystemEventType::ShutdownRequest);
			break;
		}
	}

	
	return SystemEventResult::Ok;
}

SystemEventResult ClearScreen(void* system, SystemEventType, SystemEventData)
{
	PlatformSystem* platform = (PlatformSystem*)system;
	SDL_GL_SwapWindow(platform->window);

	return SystemEventResult::Ok;
}


MistNamespaceEnd
