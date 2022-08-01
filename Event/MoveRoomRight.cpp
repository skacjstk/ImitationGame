#include "ImitationGame/framework.h"
#include "Event.h"
#include "MoveRoomRight.h"

void MoveRoomRight::execute()
{
	SCENEMANAGER->GetCurrentScene()->MoveRoom(1, 0);
}
