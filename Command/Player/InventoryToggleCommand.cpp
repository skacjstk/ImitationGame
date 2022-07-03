#include "ImitationGame/framework.h"
#include "Base/GameActor.h"
#include "Object/Player.h"
#include "InventoryToggleCommand.h"

void InventoryToggleCommand::execute(GameActor& actor)
{
	actor.InventoryToggle();
}
