#include "ImitationGame/framework.h"
#include "Event.h"
#include "MoveRoomDown.h"

void MoveRoomDown::execute()
{
	SCENEMANAGER->GetCurrentScene()->MoveRoom(0, 1);
}
