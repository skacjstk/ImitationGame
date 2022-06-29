#include "ImitationGame/framework.h"
#include "./Command/LeftMoveCommand.h"
#include "./Command/RightMoveCommand.h"
#include "./Command/JumpCommand.h"
#include "InputHandler.h"

InputHandler::InputHandler()
{
	BindActorInput();
}

class Command* InputHandler::handleInput()
{
	if (KEYBOARD->Press('A')) return buttonA_;
	if (KEYBOARD->Press('W')) return buttonW_;
	if (KEYBOARD->Press('D')) return buttonD_;
//	if (KEYBOARD->Press('S')) {
//		if (KEYBOARD->Press(VK_SPACE))
//			return buttonS_SPACE;
//		else
//			return buttonS_;
//	}
	return nullptr;
}

void InputHandler::BindActorInput()
{
	buttonA_ = (Command*)new LeftMoveCommand();
	buttonD_ = (Command*)new RightMoveCommand();
	buttonW_ = (Command*)new JumpCommand();
//	buttonS_;
//	buttonS_SPACE;
}
