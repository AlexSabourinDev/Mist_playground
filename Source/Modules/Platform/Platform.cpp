
#include <Mist_Common\include\UtilityMacros.h>

#include <Platform/Platform.h>
#include <Platform\PlatformSystem.h>

#include <Systems/System.h>
#include <Utility/String.h>
#include <Utility/FileIO.h>

#include <SDL.h>

#include <cJson/cJSON.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

MistNamespace

cJSON* LoadConfig(const char* executableDir)
{
	String filePath = Create(executableDir);
	Append(&filePath, "/Config/Platform.json");
	String fileContents = ReadFile(ToCStr(&filePath));

	cJSON* configData = cJSON_Parse(fileContents.m_StringBuffer);

	Clear(&fileContents);
	Clear(&filePath);
	return configData;
}

Uint32 LoadSubsystemFlags(cJSON* config)
{
	Uint32 flags = 0;

	cJSON* systemList = cJSON_GetObjectItem(config, "Systems");

	if (cJSON_IsArray(systemList))
	{
		for (int i = 0; i < cJSON_GetArraySize(systemList); i++)
		{
			cJSON* systemName = cJSON_GetArrayItem(systemList, i);
			if (cJSON_IsString(systemName))
			{
				if (Equal(systemName->valuestring, "Video"))
				{
					flags |= SDL_INIT_VIDEO;
				}
				else if (Equal(systemName->valuestring, "Audio"))
				{
					flags |= SDL_INIT_AUDIO;
				}
				else if (Equal(systemName->valuestring, "Input"))
				{
					flags |= SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC;
				}
			}
		}
	}

	return flags;
}



SystemData InitializePlatform(SystemAllocator allocator, const char* dataPath)
{
	cJSON* config = LoadConfig(dataPath);

	if (SDL_Init(LoadSubsystemFlags(config)) != 0) {
		MistAssert(false);
	}

	cJSON* windowConfigResource = cJSON_GetObjectItem(config, "WindowConfig");
	WindowConfig windowConfig;
	Deserialize(windowConfigResource, &windowConfig);

	cJSON_Delete(config);

	return CreatePlatformSystem(allocator, windowConfig);
}

void DeinitializePlatform(SystemDeallocator deallocator, SystemData data)
{
	DestroyPlatformSystem(deallocator, (PlatformSystem*)data);
	SDL_Quit();
}

void ProvidePlatformDependencies(SystemData data, SystemEventDispatch* eventHandlers)
{
	ProvideEventSystem((PlatformSystem*)data, eventHandlers);
}

MistNamespaceEnd
