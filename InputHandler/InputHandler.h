#pragma once
#include "./Command/Command.h"
class InputHandler
{
public:
	virtual vector<Command*> handleInput() = 0;
};
