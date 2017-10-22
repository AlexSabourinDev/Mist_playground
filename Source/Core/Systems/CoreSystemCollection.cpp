
#include <Mist_Common\include\UtilityMacros.h>

#include <Systems/CoreSystemCollection.h>

MIST_NAMESPACE


// -API-
// Add a system to the system collection
__declspec(dllexport) void AddSystem(CoreSystemCollection* collection, CoreSystemType type, System system) {
	System* insertLocation = &collection->m_Systems[(int)type];
	// If we have data, the system slot is occupied!
	MIST_ASSERT(IsValid(insertLocation) == false);

	*insertLocation = system;
}

// Retrieve the correct system from the system collection
__declspec(dllexport) System* GetSystem(CoreSystemCollection* collection, CoreSystemType type) {
	System* system = &collection->m_Systems[(int)type];
	// We have to have data, or else the system isn't available
	MIST_ASSERT(IsValid(system));

	return system;
}

MIST_NAMESPACE_END