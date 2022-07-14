#pragma once
#include "Event/Event.h"
class EventHandler
{
public:
	std::queue<Event*> eventQueue;
	std::map<wstring, Event*> eventMap;
public:
	EventHandler();
	void Update();
	void Push(wstring eventName);
};
