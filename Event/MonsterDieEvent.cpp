#include "ImitationGame/framework.h"
#include "Event.h"
#include "MonsterDieEvent.h"

void MonsterDieEvent::execute()
{
	SCENEMANAGER->GetCurrentScene()->GetCurrentRoom()->DecreaseCurrentMonsterNumber();
}
