#ifdef _MSC_VER
#include <cstdlib>
#include <crtdbg.h>
#endif

#include <Mist_Common\include\UtilityMacros.h>

#include <Platform/Platform.h>
#include <Rendering/Rendering.h>
#include <RenderingPlayground\RenderingPlayground.h>

#include <Systems\System.h>

#include <Core/Systems/SystemEventHandler.h>
#include <Core/Utility/String.h>
#include <Core/Utility/FileIO.h>

#include <3rdParty/cJson/cJSON.h>

#include <cstdio>


using namespace MistNamespaceName;

void Print(const char* string)
{
	printf("Engine Log: %s\n", string);
}

String LoadDataPath(const char* executableDir)
{
	String jsonPath = Create(executableDir);
	Append(&jsonPath, "/../Paths.json");
	String fileContents = ReadFile(ToCStr(&jsonPath));

	cJSON* configData = cJSON_Parse(fileContents.m_StringBuffer);

	String path = Create(cJSON_GetObjectItem(configData, "DataPath")->valuestring);
	cJSON_Delete(configData);

	Clear(&fileContents);
	Clear(&jsonPath);

	String filePath = Create(executableDir);
	Append(&filePath, &path);

	Clear(&path);
	return filePath;
}

int main(int argc, char *argv[])
{
	String dataPath = LoadDataPath(argv[0]);

	SystemEventDispatch systemEvents;

	Print("Initializing systems");
	SystemData platformData = InitializePlatform(malloc, ToCStr(&dataPath));
	SystemData rendererData = InitializeRenderer(malloc);
	SystemData renderingPlayground = InitializeRenderingPlayground(malloc, ToCStr(&dataPath));

	ProvidePlatformDependencies(platformData, &systemEvents);
	ProvideRenderingDependencies(rendererData, &systemEvents);
	ProvideRenderingPlaygroundDependencies(renderingPlayground, &systemEvents, GetRenderer(rendererData));

	// Assure that the engine is shutdown when receiving a shutdown event
	bool lifetimeFlag = true;

	RegisterHandler(&systemEvents, SystemEventType::ShutdownRequest,
		[](void* flag, SystemEventType, SystemEventData)
		{
			Print("Shutting down engine");

			bool* lifetimeFlag = (bool*)flag;
			*lifetimeFlag = false;
			return SystemEventResult::Ok;
		}, &lifetimeFlag);


	DispatchEvent(&systemEvents, SystemEventType::Startup);

	while (lifetimeFlag)
	{
		DispatchEvent(&systemEvents, SystemEventType::Tick);
	}

	DispatchEvent(&systemEvents, SystemEventType::Shutdown);


	Print("Deinitializing systems");
	DeinitializeRenderingPlayground(free, renderingPlayground);
	DeinitializeRenderer(free, rendererData);
	DeinitializePlatform(free, platformData);

	Clear(&dataPath);

#ifdef _MSC_VER
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}