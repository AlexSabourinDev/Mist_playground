#include <Mist_Common\include\UtilityMacros.h>

#include <Systems\System.h>

MIST_NAMESPACE

__declspec(dllexport) bool IsValid(System* system)
{
	return system->m_Initialize != nullptr;
}


MIST_NAMESPACE_END
