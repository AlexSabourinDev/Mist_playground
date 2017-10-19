
#include <Mist_Common\include\UtilityMacros.h>

#include <Systems/CoreSystemCollection.h>

MIST_NAMESPACE


// -API-
// Add a system to the system collection
void AddSystem(CoreSystemCollection* collection, CoreSystemType type, System system) {
	System* insertLocation = &collection->m_Systems[(int)type];
	// If we have data, the system slot is occupied!
	MIST_ASSERT(insertLocation->m_Data == nullptr);

	insertLocation->m_Data = system.m_Data;
}

// Retrieve the correct system from the system collection
System* GetSystem(CoreSystemCollection* collection, CoreSystemType type) {
	System* system = &collection->m_Systems[(int)type];
	// We have to have data, or else the system isn't available
	MIST_ASSERT(system->m_Data != nullptr);

	return system;
}

MIST_NAMESPACE_END