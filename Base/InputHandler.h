#pragma once
#include "./Command/Command.h"
class InputHandler
{
public:
	InputHandler();
	Command* handleInput();
	virtual ~InputHandler() {};
public:	// ��� bind �� 
	void BindActorInput();
private:
	Command* buttonA_;
	Command* buttonD_;
	Command* buttonS_;
	Command* buttonS_SPACE;
	Command* buttonW_;
};
