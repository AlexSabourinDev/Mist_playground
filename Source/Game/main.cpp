#ifdef _MSC_VER
#include <cstdlib>
#include <crtdbg.h>
#endif

#include <Mist_Common\include\UtilityMacros.h>
#include <Mist_Profiler\Mist_Profiler.h>

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

	cJSON* configData = cJSON_Parse(fileContents.stringBuffer);

	String path = Create(cJSON_GetObjectItem(configData, "DataPath")->valuestring);
	cJSON_Delete(configData);

	Clear(&fileContents);
	Clear(&jsonPath);

	String filePath = Create(executableDir);
	Append(&filePath, &path);

	Clear(&path);
	return filePath;
}

constexpr size_t AllocationSize = 1024 * 100000;
struct StackAllocator
{
	uint8_t data[AllocationSize];
	size_t nextAllocationLocation;
};

SystemAllocator Create(StackAllocator* stackAllocator)
{
	SystemAllocator allocator;
	allocator.allocatorData = stackAllocator;
	allocator.allocate = [](void* allocatorData, size_t allocationSize)->void*
	{
		StackAllocator* allocator = (StackAllocator*)allocatorData;
		if (allocator->nextAllocationLocation + allocationSize > AllocationSize)
		{
			MistAssert(false);
			return nullptr;
		}
		void* allocationLocation = &allocator->data[allocator->nextAllocationLocation];
		allocator->nextAllocationLocation += allocationSize;
		return allocationLocation;
	};
	// We don't do anything in deallocation, systems are deallocated at program termination
	allocator.deallocate = [](void* allocatorData, void* data) {};
	return allocator;
}

#define MIST_PROFILE_GROUP(cat, name, ...) \
	MIST_BEGIN_PROFILE(cat, name); \
	__VA_ARGS__ \
	MIST_END_PROFILE(cat, name);

MIST_PROFILE_DEFINE_GLOBALS
int main(int argc, char *argv[])
{
	Mist_ProfileInit();

	String dataPath = LoadDataPath(argv[0]);

	// Allocator
	StackAllocator* stackAllocator = (StackAllocator*)malloc(sizeof(StackAllocator));
	stackAllocator->nextAllocationLocation = 0;

	SystemAllocator systemAllocator = Create(stackAllocator);

	SystemEventDispatch systemEvents;

	MIST_BEGIN_PROFILE("Game::Main", "Initialization");

	MIST_PROFILE_GROUP("Game::Main", "Platform System",
		SystemData platformData = InitializePlatform(systemAllocator, ToCStr(&dataPath)););

	MIST_PROFILE_GROUP("Game::Main", "Renderer System",
		SystemData rendererData = InitializeRenderer(systemAllocator););

	MIST_PROFILE_GROUP("Game::Main", "Rendering Playground",
		SystemData renderingPlayground = InitializeRenderingPlayground(systemAllocator, ToCStr(&dataPath)););

	MIST_BEGIN_PROFILE("Game::Main", "Providing Dependencies");
	ProvidePlatformDependencies(platformData, &systemEvents);
	ProvideRenderingDependencies(rendererData, &systemEvents);
	ProvideRenderingPlaygroundDependencies(renderingPlayground, &systemEvents, GetRenderer(rendererData));
	MIST_END_PROFILE("Game::Main", "Providing Dependencies");

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

	MIST_END_PROFILE("Game::Main", "Initialization");


	MIST_BEGIN_PROFILE("Game::Main", "Startup");
	DispatchEvent(&systemEvents, SystemEventType::Startup);
	MIST_END_PROFILE("Game::Main", "Startup");

	while (lifetimeFlag)
	{
		MIST_BEGIN_PROFILE("Game::Main", "Tick");
		DispatchEvent(&systemEvents, SystemEventType::Tick);
		MIST_END_PROFILE("Game::Main", "Tick");
	}

	MIST_BEGIN_PROFILE("Game::Main", "Shutdown");
	DispatchEvent(&systemEvents, SystemEventType::Shutdown);
	MIST_END_PROFILE("Game::Main", "Shutdown");


	MIST_BEGIN_PROFILE("Game::Main", "Deinitialization");
	DeinitializeRenderingPlayground(systemAllocator, renderingPlayground);
	DeinitializeRenderer(systemAllocator, rendererData);
	DeinitializePlatform(systemAllocator, platformData);

	Clear(&dataPath);

	free(stackAllocator);
	MIST_END_PROFILE("Game::Main", "Deinitialization");

#ifdef _MSC_VER
	_CrtDumpMemoryLeaks();
#endif

	// Flush all of that sweet, sweet profiling data
	Mist_FlushThreadBuffer();

	String flush = Create(mist_ProfilePreface);
	char* print = Mist_Flush();
	Append(&flush, print);
	free(print);
	Append(&flush, mist_ProfilePostface);

	String filePath = Create(argv[0]);
	Append(&filePath, "/../MistRenderingPlayground.Profile");

	Mist::WriteFile(ToCStr(&filePath), &flush);
	Clear(&filePath);
	Clear(&flush);

	Mist_ProfileTerminate();
	
	return 0;
}