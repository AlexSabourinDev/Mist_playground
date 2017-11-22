#ifdef _MSC_VER
#include <cstdlib>
#include <crtdbg.h>
#endif

#include <Mist_Common\include\UtilityMacros.h>

#include <Platform/Platform.h>
#include <Rendering/Rendering.h>

#include <Systems\System.h>

#include <Core/Utility/SystemEventHandler.h>

#include <3rdParty/cJson/cJSON.h>

#include <cstdio>


using namespace MIST_NAMESPACE_NAME;

void Print(const char* string)
{
	printf("Engine Log: %s\n", string);
}


int main(int argc, char *argv[])
{
	SystemEventDispatch systemEvents;

	Print("Initializing systems");
	SystemData platformData = InitializePlatform(malloc, argc, argv);
	SystemData rendererData = InitializeRenderer(malloc);

	ProvidePlatformDependencies(platformData, &systemEvents);
	ProvideRenderingDependencies(rendererData, &systemEvents);

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
	DeinitializePlatform(free, platformData);
	DeinitializeRenderer(free, rendererData);

#ifdef _MSC_VER
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}