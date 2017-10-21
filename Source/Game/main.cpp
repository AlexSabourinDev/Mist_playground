#include <Mist_Common\include\UtilityMacros.h>

#include <Systems\CoreSystemCollection.h>

#include <Utility/DynamicArray.h>
#include <Utility/String.h>
#include <Utility/FileIO.h>

#include <Mist_plugin-loader\include\Plugin.h>

#include <3rdParty/cJson/cJSON.h>

#include <cstdio>

using namespace MIST_NAMESPACE_NAME;

DynamicArray<Plugin::Handle> LoadPlugins(DynamicArray<String> pluginNames)
{
	DynamicArray<Plugin::Handle> loadedPlugins;
	Reserve(&loadedPlugins, (size_t)CoreSystemType::Max);
	
	for (size_t i = 0; i < pluginNames.m_Size; i++)
	{
		Plugin::Handle plugin = Plugin::Load(ToCStr(&pluginNames.m_Data[i]));
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

	return pluginNames;
}

int main(int argc, char *argv[])
{
	CoreSystemCollection systemCollection;

	DynamicArray<String> pluginIDs = LoadPluginConfig(argv[0]);
	DynamicArray<Plugin::Handle> plugins = LoadPlugins(pluginIDs);



	UnloadPlugins(&plugins);
	Clear(&pluginIDs);

	return 0;
}