#include "ImitationGame/framework.h"
#include "Event/EnterDungeonEvent.h"
#include "Event/SwapItemEvent.h"
//#include "Event/Event.h"
#include "EventHandler.h"

EventHandler::EventHandler()
{
	eventMap.insert(std::make_pair<wstring, Event*>(L"EnterDungeon", new EnterDungeonEvent()));
	eventMap.insert(std::make_pair<wstring, Event*>(L"SwapItem", new SwapItemEvent()));
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
