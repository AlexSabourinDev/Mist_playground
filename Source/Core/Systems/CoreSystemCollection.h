#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <Systems\System.h>
#include <Systems/CoreSystemTypes.h>

MIST_NAMESPACE

struct CoreSystemCollection;

// -API-
// Add a system to the system collection
void AddSystem(CoreSystemCollection* collection, CoreSystemType type, System system);
// Retrieve the correct system from the system collection
System* GetSystem(CoreSystemCollection* collection, CoreSystemType type);

MIST_NAMESPACE_END