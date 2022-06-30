#pragma once
#include "./Command/Command.h"
class InputHandler
{
public:
	InputHandler();
	vector<class Command*> handleInput();
	virtual ~InputHandler() {};
public:	// ��� bind �� 
	void BindActorInput();
private:
	Command* buttonA_;
	Command* buttonD_;
	Command* buttonS_;
	Command* buttonS_SPACE;
	Command* buttonW_;
	Command* idleCommand_;
	Command* mouse0_;
};
