#include "ImitationGame/framework.h"
#include "./Base/GameActor.h"
#include "DashCommand.h"

void DashCommand::execute(GameActor& actor)
{
	actor.Dash();
}
