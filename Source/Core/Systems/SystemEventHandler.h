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
	ScreenSize,
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

	static constexpr size_t MaxSystemEventHandlers = 10;

	struct Handler
	{
		Call call;
		void* m_Data;
	};

	Handler registeredHandlers[MaxSystemEventHandlers];
	size_t registeredCount = 0;
};


struct SystemEventDispatch
{
	SystemEventHandler handlers[(size_t)SystemEventType::Max];
	static constexpr size_t size = (size_t)SystemEventType::Max;
};

void RegisterHandler(SystemEventDispatch* eventHandlers, SystemEventType eventType, SystemEventHandler::Call handler, void* data);
void DispatchEvent(SystemEventDispatch* eventHandlers, SystemEventType eventType, SystemEventData eventData = nullptr);

MistNamespaceEnd
