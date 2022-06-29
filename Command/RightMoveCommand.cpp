#include "ImitationGame/framework.h"
#include "Base/GameActor.h"
#include "RightMoveCommand.h"

void RightMoveCommand::execute(GameActor & actor)
{
	actor.RightMove();
}
