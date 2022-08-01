#include "ImitationGame/framework.h"
#include "Event.h"
#include "MoveRoomUp.h"

void MoveRoomUp::execute()
{
	SCENEMANAGER->GetCurrentScene()->MoveRoom(0, -1);
}
