#include "ImitationGame/framework.h"
#include "Base/GameActor.h"
#include "Object/Player.h"
#include "InteractionCommand.h"

void InteractionCommand::execute(GameActor & actor)
{
	Player* pointer = (Player*)&actor;
	if (pointer->GetInteractionTarget() != nullptr)
	{
		pointer->GetInteractionTarget()->Communicate();
	}
}
