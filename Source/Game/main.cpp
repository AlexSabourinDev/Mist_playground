#include <Mist_Common\include\UtilityMacros.h>

#include <Systems\CoreSystemCollection.h>

#include <Utility/DynamicArray.h>
#include <Utility/String.h>

#include <Mist_plugin-loader\include\Plugin.h>

#include <cstdio>

using namespace MIST_NAMESPACE_NAME;

void LoadPlugins(DynamicArray<Plugin::Handle>* plugins)
{

}

void UnloadPlugins(DynamicArray<Plugin::Handle>* plugins)
{
	for (size_t i = 0; i < plugins->m_Size; i++)
	{
		Plugin::Unload(plugins->m_Data[i]);
	}

	Clear(plugins);
}

int main(int argc, char *argv[])
{
	CoreSystemCollection systemCollection;

	DynamicArray<Plugin::Handle> plugins;
	Reserve(&plugins, (size_t)CoreSystemType::Max);


	UnloadPlugins(&plugins);
	return 0;
}