#include "ImitationGame/framework.h"
#include "./InputHandler/InputHandler.h"
#include "./Command/Inventory/DragAndDropCommand.h"
#include "InventoryInputHandler.h"

InventoryInputHandler::InventoryInputHandler()
{
	BindActorInput();
}

vector<class Command*> InventoryInputHandler::handleInput()
{
	vector<class Command*> tempCommand;
	if (KEYBOARD->Down(VK_LCONTROL)) tempCommand.push_back(buttonLCONTROL_);
	return tempCommand;
}

void InventoryInputHandler::BindActorInput()
{
	buttonLCONTROL_ = (Command*)new DragAndDropCommand();
}
