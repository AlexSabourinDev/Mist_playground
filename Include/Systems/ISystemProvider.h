#pragma once

#include <Mist_Common\/include\UtilityMacros.h>
#include "ISystem.h"

#include <string>

MIST_NAMESPACE

// Provides a system based on the requested typeinfo
class ISystemProvider {
public:
	virtual ISystem* RetrieveSystem(const std::string& systemName) const = 0;
};

MIST_NAMESPACE_END