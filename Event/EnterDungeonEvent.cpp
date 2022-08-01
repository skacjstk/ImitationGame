#include "ImitationGame/framework.h"
#include "Event.h"
#include "EnterDungeonEvent.h"

void EnterDungeonEvent::execute()
{
	SCENEMANAGER->GetCurrentScene()->ExitScene();
	SCENEMANAGER->ChangeScene("Floor_1");
}
