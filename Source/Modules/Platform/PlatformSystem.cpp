
#include <Mist_Common\include\UtilityMacros.h>

#include <Platform\PlatformSystem.h>
#include <Utility/BitManipulations.h>
#include <Utility/String.h>

#include <Math\Serialization.h>

#include <SDL.h>

#include <cstdlib>
#include <cstdio>

MIST_NAMESPACE

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

	return sdlFlags;
}

void InitializePlatformSystem(System* system)
{
	PlatformSystem* platformSystem = (PlatformSystem*)system->m_Data;

	if (SDL_WasInit(SDL_INIT_VIDEO))
	{
		WindowConfig* config = &platformSystem->m_Config;
		SDL_Window* window = SDL_CreateWindow(ToCStr(&config->m_WindowName), config->m_Rect.x, config->m_Rect.y,
			config->m_Rect.width, config->m_Rect.height, ConvertWindowFlags(config->m_WindowFlags));
		MIST_ASSERT(window != nullptr);

		platformSystem->m_Window = window;
	}
}

void DeinitializePlatformSystem(System* system)
{
	PlatformSystem* platform = (PlatformSystem*)system->m_Data;
	SDL_DestroyWindow(platform->m_Window);
	FreeConfig(&platform->m_Config);
	free(system->m_Data);
}

void TickPlatformSystem(System* system)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) 
	{

	}
}

System CreatePlatformSystem(WindowConfig config)
{
	System platformSystem;

	PlatformSystem* platform = (PlatformSystem*)malloc(sizeof(PlatformSystem));
	platform->m_Config = config;

	platformSystem.m_Data = platform;
	platformSystem.m_Initialize = &InitializePlatformSystem;
	platformSystem.m_SystemTick = &TickPlatformSystem;
	platformSystem.m_Deinitialize = &DeinitializePlatformSystem;

	return platformSystem;
}


MIST_NAMESPACE_END