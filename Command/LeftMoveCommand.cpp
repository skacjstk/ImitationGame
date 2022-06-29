#include "ImitationGame/framework.h"
#include "Base/GameActor.h"
#include "LeftMoveCommand.h"

void LeftMoveCommand::execute(GameActor & actor)
{
	actor.LeftMove();
}
