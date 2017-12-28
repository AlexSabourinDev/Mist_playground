#include <Mist_Common\include\UtilityMacros.h>
#include <Core/Systems/SystemEventHandler.h>

MistNamespace

void RegisterHandler(SystemEventDispatch* eventHandlers, SystemEventType eventType, SystemEventHandler::Call handler, void* data)
{
	SystemEventHandler::Handler systemHandler;
	systemHandler.m_Call = handler;
	systemHandler.m_Data = data;

	SystemEventHandler* eventHandler = &eventHandlers->m_Handlers[(uint32_t)eventType];
	eventHandler->m_RegisteredHandlers[eventHandlers->m_Handlers[(uint32_t)eventType].m_RegisteredCount] = systemHandler;
	eventHandler->m_RegisteredCount++;
}


void DispatchEvent(SystemEventDispatch* eventHandlers, SystemEventType eventType, SystemEventData eventData)
{
	SystemEventHandler* eventHandler = &eventHandlers->m_Handlers[(uint32_t)eventType];
	
	for (size_t i = 0; i < eventHandler->m_RegisteredCount; i++)
	{
		SystemEventHandler::Handler* handler = &eventHandler->m_RegisteredHandlers[i];
		handler->m_Call(handler->m_Data, eventType, eventData);
	}
}

MistNamespaceEnd
