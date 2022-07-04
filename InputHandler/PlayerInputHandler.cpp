#include "ImitationGame/framework.h"
#include "InputHandler/InputHandler.h"
#include "./Command/LeftMoveCommand.h"
#include "./Command/RightMoveCommand.h"
#include "./Command/JumpCommand.h"
#include "./Command/IdleCommand.h"
#include "./Command/AttackCommand.h"
#include "./Command/Player/InventoryToggleCommand.h"
#include "PlayerInputHandler.h"

PlayerInputHandler::PlayerInputHandler()
{
	BindActorInput();
}

vector<class Command*> PlayerInputHandler::handleInput()
{
	vector<class Command*> tempCommand;
	if (KEYBOARD->Press('A')) tempCommand.push_back(buttonA_);
	if (KEYBOARD->Press('D')) tempCommand.push_back(buttonD_);
	if (KEYBOARD->Press('W')) tempCommand.push_back(buttonW_);
	if (KEYBOARD->Down('V')) tempCommand.push_back(buttonV_);
	//	if (KEYBOARD->Press('S')) {
	//		if (KEYBOARD->Press(VK_SPACE))
	//			return buttonS_SPACE;
	//		else
	//			return buttonS_;
	//	}
	if (Mouse->Down(0)) tempCommand.push_back(mouse0_);	// Next: UI 형태에 따라 바뀌어야 할 수 있음.
	if (tempCommand.size() == 0)
		tempCommand.push_back(idleCommand_);
	return tempCommand;
}

void PlayerInputHandler::BindActorInput()
{
	buttonA_ = (Command*)new LeftMoveCommand();
	buttonD_ = (Command*)new RightMoveCommand();
	buttonW_ = (Command*)new JumpCommand();
	buttonV_ = (Command*)new InventoryToggleCommand();
	idleCommand_ = (Command*)new IdleCommand();
	mouse0_ = (Command*)new AttackCommand();

//	buttonS_;
//	buttonS_SPACE;
}
