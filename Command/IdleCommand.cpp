#include "ImitationGame/framework.h"
#include "Base/GameActor.h"
#include "IdleCommand.h"

void IdleCommand::execute(GameActor & actor)
{
	actor.Idle();
}
