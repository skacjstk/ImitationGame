#include "ImitationGame/framework.h"
#include "Base/GameActor.h"
#include "JumpCommand.h"

void JumpCommand::execute(GameActor & actor)
{
	actor.Jump();
}
