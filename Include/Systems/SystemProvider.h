#pragma once

#include <Mist_Common/include\UtilityMacros.h>
#include "SystemCollection.h"

#include <string>

MIST_NAMESPACE

// Provides a system based on the requested typeinfo
class SystemProvider {
public:
	inline SystemProvider(const SystemCollection& systems);

	template< typename SystemType >
	SystemType* RetrieveSystem() const;
private:
	const SystemCollection& m_Systems;
};


// -Implementation-

inline SystemProvider::SystemProvider(const SystemCollection& systems) 
	: m_Systems(systems) {
}

template< typename SystemType >
SystemType* SystemProvider::RetrieveSystem() const {
	for (size_t i = 0; i < m_Systems.Size(); i++) {
		SystemType* retrievedSystem = dynamic_cast<SystemType*>(m_Systems[i]);
		if (retrievedSystem != nullptr) {
			return retrievedSystem;
		}
	}
	return nullptr;
}


MIST_NAMESPACE_END