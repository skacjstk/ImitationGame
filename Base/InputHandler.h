#pragma once
#include "./Command/Command.h"
class InputHandler
{
public:
	InputHandler();
	Command* handleInput();
	virtual ~InputHandler() {};
public:	// ¸í·É bind ¿ë 
	void BindActorInput();
private:
	Command* buttonA_;
	Command* buttonD_;
	Command* buttonS_;
	Command* buttonS_SPACE;
	Command* buttonW_;
};
