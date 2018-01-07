#pragma once

#include "ConfigMacros.h"

#include <Windows.h>


// Define a utility macro to define multiple optional namespaces.
// @Details: This decision was made in order to support nesting namespaces 
//			in later files and modules
#if MistShouldUseNamespaces

#define MistNamespace \
namespace MistNamespaceName {

#define MistNamespaceEnd }

#else

#define MistNamespace
#define NamespaceEnd

#endif

// Define the assertion method for assertions
#include <cassert>
#define MistAssert(condition) do { if(condition == false) { DebugBreak(); } } while(false)
#define MistUnused(data) (void)data;