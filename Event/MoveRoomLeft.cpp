#include "ImitationGame/framework.h"
#include "Event.h"
#include "MoveRoomLeft.h"

void MoveRoomLeft::execute()
{
	SCENEMANAGER->GetCurrentScene()->MoveRoom(-1, 0);
}
