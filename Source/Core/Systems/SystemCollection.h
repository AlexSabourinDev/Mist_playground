#pragma once

#include <Mist_Common\include\UtilityMacros.h>
#include "ISystem.h"

#include <cstdint>

MIST_NAMESPACE

// Keep track of all the active systems
class SystemCollection {
public:
	inline void AddSystem(ISystem* system);
	inline void RemoveSystem(ISystem* system);

	inline ISystem* operator[](size_t index) const;
	inline size_t Size() const;

private:
	static const size_t MAX_SYSTEM_COUNT = 5;

	ISystem* m_Systems[MAX_SYSTEM_COUNT] = { nullptr };
	size_t m_Size = 0;
};


// -Implementation

inline void SystemCollection::AddSystem(ISystem* system) {
	// You've run out of space for systems, are there too many or do you need more space?
	MIST_ASSERT(m_Size < MAX_SYSTEM_COUNT);
	// Someone passed in a nullptr, how come?
	MIST_ASSERT(system != nullptr);

	m_Systems[m_Size] = system;
	m_Size++;
}

inline void SystemCollection::RemoveSystem(ISystem* system) {
	MIST_ASSERT(system != nullptr);

	bool systemRemoved = false;
	for (size_t i = 0; i < m_Size; i++) {
		if (systemRemoved) {
			m_Systems[i - 1] = m_Systems[i];
		}
		else if (m_Systems[i] == system) {
			m_Systems[i] = nullptr;
			systemRemoved = true;
		}
	}

	MIST_ASSERT(systemRemoved);

	m_Size--;
}

inline ISystem* SystemCollection::operator[](size_t index) const {
	MIST_ASSERT(index < m_Size);
	return m_Systems[index];
}

inline size_t SystemCollection::Size() const {
	return m_Size;
}

MIST_NAMESPACE_END
