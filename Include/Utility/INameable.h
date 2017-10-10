#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <string>

MIST_NAMESPACE

class INameable {
public:
	virtual ~INameable() = default;
	virtual const std::string& GetName() const = 0;
};

MIST_NAMESPACE_END