#pragma once

#include <Mist_Common/include/UtilityMacros.h>
#include "../Utility/INameable.h"

#include <string>

MIST_NAMESPACE

class ISystem : public INameable {

public:
	// -INameable API-
	virtual const std::string& GetName() const override = 0;

};

MIST_NAMESPACE_END
