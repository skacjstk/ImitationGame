#include "ImitationGame/framework.h"
#include "./Command/LeftMoveCommand.h"
#include "./Command/RightMoveCommand.h"
#include "./Command/JumpCommand.h"
#include "./Command/IdleCommand.h"
#include "./Command/AttackCommand.h"
#include "./Command/DashCommand.h"
#include "./Command/UnderJumpCommand.h"
#include "./Command/Player/InventoryToggleCommand.h"
#include "./Command/Player/SwapHandFocusCommand.h"
#include "./Object/Player.h"
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
	if (KEYBOARD->Down(VK_OEM_3)) tempCommand.push_back(buttonVK_OEM_3_);
	if (KEYBOARD->Press(VK_SPACE)) {
		if (KEYBOARD->Press('S'))
			tempCommand.push_back(buttonS_SPACE);
		else
			tempCommand.push_back(buttonW_);
	}
	if (Mouse->Down(0)) tempCommand.push_back(mouse0_);	// Next: UI ���¿� ���� �ٲ��� �� �� ����.
	if (Mouse->Down(2)) tempCommand.push_back(mouse2_);	// 2���� ��Ŭ�� ����.
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
	buttonVK_OEM_3_ = (Command*) new SwapHandFocusCommand();
	mouse2_ = (Command*)new DashCommand();
	buttonS_SPACE = (Command*)new UnderJumpCommand();
	//buttonVK_OEM_3_ = bind(&Player::SwapHandFocus, ); // Player�� �ν��Ͻ��� �־���� ��.

//	buttonS_;
//	buttonS_SPACE;
}
