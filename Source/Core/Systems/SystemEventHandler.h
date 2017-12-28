#pragma once

#include <Mist_Common\include\UtilityMacros.h>
#include <Systems\System.h>
#include <cstdint>

MistNamespace

enum class SystemEventType
{
	Startup,
	Tick,
	ClearScreen,
	ShutdownRequest,
	Shutdown,
	Max
};

enum class SystemEventResult
{
	Ok,
	Consume,
	Error
};


using SystemEventData = void*;

struct SystemEventHandler
{
	using Call = SystemEventResult(*)(void* data, SystemEventType eventType, SystemEventData eventData);

	static constexpr size_t MAX_SYSTEM_EVENT_HANDLERS = 10;

	struct Handler
	{
		Call m_Call;
		void* m_Data;
	};

	Handler m_RegisteredHandlers[MAX_SYSTEM_EVENT_HANDLERS];
	size_t m_RegisteredCount = 0;
};


struct SystemEventDispatch
{
	SystemEventHandler m_Handlers[(size_t)SystemEventType::Max];
	static constexpr size_t m_Size = (size_t)SystemEventType::Max;
};

void RegisterHandler(SystemEventDispatch* eventHandlers, SystemEventType eventType, SystemEventHandler::Call handler, void* data);
void DispatchEvent(SystemEventDispatch* eventHandlers, SystemEventType eventType, SystemEventData eventData = nullptr);

MistNamespaceEnd
