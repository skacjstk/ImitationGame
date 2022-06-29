#include "ImitationGame/framework.h"
#include "./Command/LeftMoveCommand.h"
#include "./Command/RightMoveCommand.h"
#include "./Command/JumpCommand.h"
#include "./Command/IdleCommand.h"
#include "InputHandler.h"

InputHandler::InputHandler()
{
	BindActorInput();
}

vector<class Command*> InputHandler::handleInput()
{
	vector<class Command*> tempCommand;
	if (KEYBOARD->Press('A')) tempCommand.push_back(buttonA_);
	if (KEYBOARD->Press('D')) tempCommand.push_back(buttonD_);
	if (KEYBOARD->Press('W')) tempCommand.push_back(buttonW_);
	//	if (KEYBOARD->Press('S')) {
	//		if (KEYBOARD->Press(VK_SPACE))
	//			return buttonS_SPACE;
	//		else
	//			return buttonS_;
	//	}
	if (tempCommand.size() == 0)
		tempCommand.push_back(idleCommand_);
	return tempCommand;
}

void InputHandler::BindActorInput()
{
	buttonA_ = (Command*)new LeftMoveCommand();
	buttonD_ = (Command*)new RightMoveCommand();
	buttonW_ = (Command*)new JumpCommand();
	idleCommand_ = (Command*)new IdleCommand();
//	buttonS_;
//	buttonS_SPACE;
}
