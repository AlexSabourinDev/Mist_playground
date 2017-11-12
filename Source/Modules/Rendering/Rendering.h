#pragma once

#include <Mist_Common/include/UtilityMacros.h>

#include <Systems\CoreSystemCollection.h>

MIST_NAMESPACE

extern "C"
{
	__declspec(dllexport) SystemArray Initialize(int argc, char *argv[]);
	__declspec(dllexport) void ReleaseSystemArray(SystemArray systems);
	__declspec(dllexport) void Deinitialize();
}

MIST_NAMESPACE_END

