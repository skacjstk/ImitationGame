#include "ImitationGame/framework.h"
#include "Event.h"
#include "RiseNextFloorEvent.h"

void RiseNextFloorEvent::execute()
{
	printf("������ ������: ȣ�� �� �ش� ������ �̸��� �������� ��\n");
	SCENEMANAGER->ChangeScene(SCENEMANAGER->GetCurrentScene()->GetName());
}
