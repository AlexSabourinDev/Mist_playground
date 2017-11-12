#pragma once

#include <Mist_Common\include\UtilityMacros.h>
#include <Systems\System.h>
#include <cstdint>

MIST_NAMESPACE

enum class SystemEventType
{
	Startup,
	Tick,
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
	using Call = SystemEventResult(*)(System* system, SystemEventType eventType, SystemEventData eventData);

	static constexpr size_t MAX_SYSTEM_EVENT_HANDLERS = 10;

	struct Handler
	{
		Call m_Call;
		System* m_System;
	};

	Handler m_RegisteredHandlers[MAX_SYSTEM_EVENT_HANDLERS];
	size_t m_RegisteredCount = 0;
};


struct SystemEventHandlers
{
	SystemEventHandler m_Handlers[(size_t)SystemEventType::Max];
	static constexpr size_t m_Size = (size_t)SystemEventType::Max;
};

__declspec(dllexport) void RegisterHandler(SystemEventHandlers* eventHandlers, SystemEventType eventType, SystemEventHandler::Call handler, System* handlerSystem);
__declspec(dllexport) void DispatchEvent(SystemEventHandlers* eventHandlers, SystemEventType eventType, SystemEventData eventData = nullptr);

MIST_NAMESPACE_END
