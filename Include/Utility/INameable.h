#pragma once

#include <Mist_Common\include\UtilityMacros.h>

#include <string>

MIST_NAMESPACE

class INameable {
public:
	virtual const std::string& GetName() const = 0;
};

MIST_NAMESPACE_END