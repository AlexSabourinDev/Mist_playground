#include <Mist_Common\include\UtilityMacros.h>
#include <Core/Systems/SystemEventHandler.h>

MistNamespace

void RegisterHandler(SystemEventDispatch* eventHandlers, SystemEventType eventType, SystemEventHandler::Call handler, void* data)
{
	SystemEventHandler::Handler systemHandler;
	systemHandler.call = handler;
	systemHandler.m_Data = data;

	SystemEventHandler* eventHandler = &eventHandlers->handlers[(uint32_t)eventType];
	eventHandler->registeredHandlers[eventHandlers->handlers[(uint32_t)eventType].registeredCount] = systemHandler;
	eventHandler->registeredCount++;
}


void DispatchEvent(SystemEventDispatch* eventHandlers, SystemEventType eventType, SystemEventData eventData)
{
	SystemEventHandler* eventHandler = &eventHandlers->handlers[(uint32_t)eventType];
	
	for (size_t i = 0; i < eventHandler->registeredCount; i++)
	{
		SystemEventHandler::Handler* handler = &eventHandler->registeredHandlers[i];
		handler->call(handler->m_Data, eventType, eventData);
	}
}

MistNamespaceEnd
