#pragma once

#include "common/common.h"
#include "event.h"

/// Bind a member function of a class to an event.
#define BIND_EVENT_FUNCTION(stringEventType, function) EventManager::GetSingleton()->addListener(stringEventType, function)
/// Bind a global function to an event.
#define BIND_EVENT_MEMBER_FUNCTION(stringEventType, classFunction) EventManager::GetSingleton()->addListener(stringEventType, [this](const Event* event) -> Variant { return this->classFunction(event); })

/// Number of event queues used to organise events in EventManager.
const unsigned int EVENTMANAGER_NUM_QUEUES = 2;

/// Function object for storing a function that handles an event.
typedef Function<Variant(const Event*)> EventFunction;

/// An Event dispatcher and registrar that also allows looking up registered events.
class EventManager
{
	Vector<Function<void()>> m_DeferList;
	HashMap<Event::Type, Vector<EventFunction>> m_EventListeners;
	Vector<Ref<Event>> m_Queues[EVENTMANAGER_NUM_QUEUES];
	unsigned int m_ActiveQueue;

	EventManager();
	~EventManager() = default;

public:
	static EventManager* GetSingleton();

	enum Constant
	{
		Infinite = 0xffffffff
	};

	void defer(Function<void()> function);
	/// Add an event. Returns false if it already exists.
	bool addEvent(const Event::Type& event);
	void removeEvent(const Event::Type& event);
	/// Add an event handler for an event. Creates a new event is not already existing. Returns false if the handler is already added.
	bool addListener(const Event::Type& type, EventFunction instance);
	/// Publish an event. Returns the result of the first event handled.
	Variant returnCall(const Event& event);
	Variant returnCall(const Event::Type& eventType, const Variant& data = 0);
	void call(const Event& event);
	void call(const Event::Type& eventType, const Variant& data = 0);
	/// Publish an event that gets evaluated the end of the current frame.
	void deferredCall(Ref<Event> event);
	void deferredCall(const Event::Type& eventType, const Variant& data = 0);
	/// Dispatch deferred events collected so far.
	bool dispatchDeferred(unsigned long maxMillis = Infinite);

	void releaseAllEventListeners();

	const HashMap<Event::Type, Vector<EventFunction>>& getRegisteredEvents() const { return m_EventListeners; }
};
