#include "ImitationGame/framework.h"
#include "Base/GameActor.h"
#include "Object/Player.h"
#include "DragAndDropCommand.h"

void DragAndDropCommand::execute(GameActor& actor)
{
	actor.ItemDragAndDrop();
}
