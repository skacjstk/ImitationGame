#include "ImitationGame/framework.h"
#include "Event/EnterDungeonEvent.h"
#include "Event/SwapItemEvent.h"
#include "Event/RiseNextFloorEvent.h"
#include "Event/MoveRoomDown.h"
#include "Event/MoveRoomLeft.h"
#include "Event/MoveRoomUp.h"
#include "Event/MoveRoomRight.h"
//#include "Event/Event.h"
#include "EventHandler.h"

EventHandler::EventHandler()
{
	eventMap.insert(std::make_pair<wstring, Event*>(L"EnterDungeon", new EnterDungeonEvent()));
	eventMap.insert(std::make_pair<wstring, Event*>(L"SwapItem", new SwapItemEvent()));
	eventMap.insert(std::make_pair<wstring, Event*>(L"RiseNextFloor", new RiseNextFloorEvent()));
	eventMap.insert(std::make_pair<wstring, Event*>(L"MR1", new MoveRoomDown()));
	eventMap.insert(std::make_pair<wstring, Event*>(L"MR2", new MoveRoomLeft()));
	eventMap.insert(std::make_pair<wstring, Event*>(L"MR3", new MoveRoomUp()));
	eventMap.insert(std::make_pair<wstring, Event*>(L"MR4", new MoveRoomRight()));
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
