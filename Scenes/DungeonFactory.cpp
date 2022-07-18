#include "./ImitationGame/framework.h"
#include "Scenes/Room.h"
#include "DungeonFactory.h"

void DungeonFactory::GenerateDungeon(Room* room)
{

	switch (room->GetRoomType())
	{
	case Room::RoomType::DISABLE:
		break;
	case Room::RoomType::NORMAL:	
		break;
	case Room::RoomType::START:		
		break;
	case Room::RoomType::END:	
		break;
	case Room::RoomType::RESTAURANT:	
		break;
	default:
		break;
	}

}
