#include "ImitationGame/framework.h"
#include "Event.h"
#include "RiseNextFloorEvent.h"

void RiseNextFloorEvent::execute()
{
	printf("다음층 오르기: 호출 전 해당 씬에서 이름을 설정해줄 것\n");
	SCENEMANAGER->GetCurrentScene()->ExitScene();
	SCENEMANAGER->ChangeScene(SCENEMANAGER->GetCurrentScene()->GetName());
}
