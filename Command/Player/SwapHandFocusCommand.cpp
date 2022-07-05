#include "ImitationGame/framework.h"
#include "./Base/GameActor.h"
#include "SwapHandFocusCommand.h"

void SwapHandFocusCommand::execute(GameActor& actor)
{
	actor.SwapHandFocus();
}
