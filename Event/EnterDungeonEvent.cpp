#include "ImitationGame/framework.h"
#include "Event.h"
#include "EnterDungeonEvent.h"

void EnterDungeonEvent::execute()
{
	printf("던전 진입 이벤트 출력됨");
	SCENEMANAGER->ChangeScene("Floor_1");
}
