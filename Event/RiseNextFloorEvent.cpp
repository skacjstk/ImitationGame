#include "ImitationGame/framework.h"
#include "Event.h"
#include "RiseNextFloorEvent.h"

void RiseNextFloorEvent::execute()
{
	printf("������ ������: ȣ�� �� �ش� ������ �̸��� �������� ��\n");
	SCENEMANAGER->GetCurrentScene()->ExitScene();
	SCENEMANAGER->ChangeScene(SCENEMANAGER->GetCurrentScene()->GetNextName());
}
