#include "ImitationGame/framework.h"
#include "./Base/GameActor.h"
#include "AttackCommand.h"

void AttackCommand::execute(GameActor& actor)
{
	actor.Attack();
}
