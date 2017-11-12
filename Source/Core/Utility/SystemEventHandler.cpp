#include <Mist_Common\include\UtilityMacros.h>
#include <Core/Utility/SystemEventHandler.h>

MIST_NAMESPACE

__declspec(dllexport) void RegisterHandler(SystemEventHandlers* eventHandlers, SystemEventType eventType, SystemEventHandler::Call handler, System* handlerSystem)
{
	SystemEventHandler::Handler systemHandler;
	systemHandler.m_Call = handler;
	systemHandler.m_System = handlerSystem;

	SystemEventHandler* eventHandler = &eventHandlers->m_Handlers[(uint32_t)eventType];
	eventHandler->m_RegisteredHandlers[eventHandlers->m_Handlers[(uint32_t)eventType].m_RegisteredCount] = systemHandler;
	eventHandler->m_RegisteredCount++;
}


__declspec(dllexport) void DispatchEvent(SystemEventHandlers* eventHandlers, SystemEventType eventType, SystemEventData eventData)
{
	SystemEventHandler* eventHandler = &eventHandlers->m_Handlers[(uint32_t)eventType];
	
	for (size_t i = 0; i < eventHandler->m_RegisteredCount; i++)
	{
		SystemEventHandler::Handler* handler = &eventHandler->m_RegisteredHandlers[i];
		handler->m_Call(handler->m_System, eventType, eventData);
	}
}

MIST_NAMESPACE_END