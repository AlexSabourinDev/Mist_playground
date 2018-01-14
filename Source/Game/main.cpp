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

constexpr size_t AllocationSize = 1024 * 5;
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

int main(int argc, char *argv[])
{
	String dataPath = LoadDataPath(argv[0]);

	// Allocator
	StackAllocator stackAllocator;
	stackAllocator.nextAllocationLocation = 0;

	SystemAllocator systemAllocator = Create(&stackAllocator);

	SystemEventDispatch systemEvents;

	Print("Initializing systems");
	SystemData platformData = InitializePlatform(systemAllocator, ToCStr(&dataPath));
	SystemData rendererData = InitializeRenderer(systemAllocator);
	SystemData renderingPlayground = InitializeRenderingPlayground(systemAllocator, ToCStr(&dataPath));

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
	DeinitializeRenderingPlayground(systemAllocator, renderingPlayground);
	DeinitializeRenderer(systemAllocator, rendererData);
	DeinitializePlatform(systemAllocator, platformData);

	Clear(&dataPath);

#ifdef _MSC_VER
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}