
#include <Mist_Common\include\UtilityMacros.h>

#include <Platform/Platform.h>
#include <Platform\PlatformSystem.h>

#include <Systems/System.h>
#include <Utility/String.h>
#include <Utility/FileIO.h>
#include <Utility/DynamicArray.h>
#include <Utility\Pair.h>

#include <SDL.h>

#include <cJson/cJSON.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>

MIST_NAMESPACE

cJSON* LoadConfig(const char* executableDir)
{
	String filePath = Create(executableDir);
	Append(&filePath, "/../Config/Platform.json");
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



PLATFORM_API SystemArray Initialize(int argc, char *argv[])
{
	cJSON* config = LoadConfig(argv[0]);

	if (SDL_Init(LoadSubsystemFlags(config)) != 0) {
		MIST_ASSERT(false);
	}

	DynamicArray<Pair<System, CoreSystemType>>* systems = (DynamicArray<Pair<System, CoreSystemType>>*)malloc(sizeof(DynamicArray<Pair<System, CoreSystemType>>));
	memset(systems, 0, sizeof(DynamicArray<Pair<System, CoreSystemType>>));

	cJSON* windowConfigResource = cJSON_GetObjectItem(config, "WindowConfig");
	WindowConfig windowConfig;
	Deserialize(windowConfigResource, &windowConfig);

	PushBack(systems, Create(CreatePlatformSystem(windowConfig), CoreSystemType::Platform));

	cJSON_Delete(config);
	return systems;
}


PLATFORM_API void ReleaseSystemArray(SystemArray data)
{
	DynamicArray<Pair<System, CoreSystemType>>* systems = (DynamicArray<Pair<System, CoreSystemType>>*)data;

	Clear(systems);
	free(systems);
}

PLATFORM_API void Deinitialize()
{
	SDL_Quit();
}

MIST_NAMESPACE_END