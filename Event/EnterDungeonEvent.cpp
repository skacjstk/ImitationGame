#include "ImitationGame/framework.h"
#include "Event.h"
#include "EnterDungeonEvent.h"

void EnterDungeonEvent::execute()
{
	printf("���� ���� �̺�Ʈ ��µ�");
	SCENEMANAGER->ChangeScene("Floor_1");
}
