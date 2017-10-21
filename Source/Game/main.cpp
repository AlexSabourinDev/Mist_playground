#ifdef _MSC_VER
#include <cstdlib>
#include <crtdbg.h>
#endif

#include <Mist_Common\include\UtilityMacros.h>

#include <Systems\CoreSystemCollection.h>

#include <Utility/DynamicArray.h>
#include <Utility/String.h>
#include <Utility/FileIO.h>
#include <Utility/Pair.h>

#include <Mist_plugin-loader\include\Plugin.h>

#include <3rdParty/cJson/cJSON.h>

#include <cstdio>


using namespace MIST_NAMESPACE_NAME;

using PluginInitialize = void(*)();
using PluginLoadSystems = void*(*)();
using PluginReleaseSystemsArray = void(*)(void* systems);
using PluginDeinitialize = void(*)();

DynamicArray<Plugin::Handle> LoadPlugins(DynamicArray<String>* pluginNames)
{
	DynamicArray<Plugin::Handle> loadedPlugins;
	Reserve(&loadedPlugins, (size_t)CoreSystemType::Max);
	
	for (size_t i = 0; i < pluginNames->m_Size; i++)
	{
		Plugin::Handle plugin = Plugin::Load(ToCStr(&pluginNames->m_Data[i]));
		MIST_ASSERT(plugin != nullptr);
		if (plugin != nullptr)
		{
			PushBack(&loadedPlugins, plugin);
		}
	}

	return loadedPlugins;
}

void UnloadPlugins(DynamicArray<Plugin::Handle>* plugins)
{
	for (size_t i = 0; i < plugins->m_Size; i++)
	{
		PluginDeinitialize deinitialize = Plugin::GetAPI<void>(plugins->m_Data[i], "Deinitialize");
		deinitialize();

		Plugin::Unload(plugins->m_Data[i]);
	}
	Clear(plugins);
}

DynamicArray<String> LoadPluginConfig(const char* executableDir)
{
	DynamicArray<String> pluginNames;

	String filePath = Create(executableDir);
	Append(&filePath, "/../Config/Plugins.json");
	String fileContents = ReadFile(ToCStr(&filePath));

	cJSON* configData = cJSON_Parse(fileContents.m_StringBuffer);
	cJSON* pluginList = cJSON_GetObjectItem(configData, "Plugins");

	if (cJSON_IsArray(pluginList))
	{
		for (int i = 0; i < cJSON_GetArraySize(pluginList); i++)
		{
			cJSON* pluginName = cJSON_GetArrayItem(pluginList, i);
			if (cJSON_IsString(pluginName))
			{
				PushBack(&pluginNames, Create(pluginName->valuestring));
			}
		}
	}

	cJSON_Delete(configData);
	Clear(&fileContents);
	Clear(&filePath);

	return pluginNames;
}

void LoadSystems(CoreSystemCollection* coreSystems, Plugin::Handle plugin)
{
	PluginInitialize pluginInitialization = Plugin::GetAPI<void>(plugin, "Initialize");
	pluginInitialization();

	PluginLoadSystems loadSystems = Plugin::GetAPI<void*>(plugin, "LoadCoreSystems");
	DynamicArray<Pair<System, CoreSystemType>>* pluginSystems = (DynamicArray<Pair<System, CoreSystemType>>*)loadSystems();

	for (size_t system = 0; system < pluginSystems->m_Size; system++)
	{
		AddSystem(coreSystems, pluginSystems->m_Data[system].m_Second, pluginSystems->m_Data[system].m_First);
	}

	PluginReleaseSystemsArray releaseSystems = Plugin::GetAPI<void, void*>(plugin, "ReleaseSystemArray");
	releaseSystems(pluginSystems);
}

int main(int argc, char *argv[])
{
	CoreSystemCollection systemCollection;

	DynamicArray<String> pluginIDs = LoadPluginConfig(argv[0]);
	DynamicArray<Plugin::Handle> plugins = LoadPlugins(&pluginIDs);

	for (size_t i = 0; i < plugins.m_Size; i++)
	{
		LoadSystems(&systemCollection, plugins.m_Data[i]);
	}

	for (size_t i = 0; i < systemCollection.m_Size; i++)
	{
		//systemCollection.m_Systems[i].m_Initialize(&systemCollection.m_Systems[i]);
	}




	for (size_t i = 0; i < systemCollection.m_Size; i++)
	{
		//systemCollection.m_Systems[i].m_Deinitialize(&systemCollection.m_Systems[i]);
	}

	UnloadPlugins(&plugins);

	for (size_t i = 0; i < pluginIDs.m_Size; i++)
	{
		Clear(&pluginIDs.m_Data[i]);
	}
	Clear(&pluginIDs);

#ifdef _MSC_VER
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}