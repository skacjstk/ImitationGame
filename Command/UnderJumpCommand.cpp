#include "ImitationGame/framework.h"
#include "Base/GameActor.h"
#include "UnderJumpCommand.h"

void UnderJumpCommand::execute(GameActor & actor)
{
	if (actor.GetIsPlatform() == true && actor.GetIsUnderJump() == false && actor.isGround_ == true)
		actor.UnderJump();
	else if(actor.isGround_ == true){
		actor.Jump();
	}
}
