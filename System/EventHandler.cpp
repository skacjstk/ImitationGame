#include "ImitationGame/framework.h"
#include "Event/EatEvent.h"
//#include "Event/Event.h"
#include "EventHandler.h"

EventHandler::EventHandler()
{
	eventMap.insert(std::pair<wstring, Event*>(L"Eat", new EatEvent()));
}

void EventHandler::Update()
{
	while(!eventQueue.empty()){
		Event* startEvent = eventQueue.front();
		startEvent->execute();
		eventQueue.pop();
	}
}

void EventHandler::Push(wstring eventName)
{
	auto tempEvent = eventMap.find(eventName);	
	if(tempEvent != eventMap.end())
		eventQueue.push(tempEvent->second);	
}
