#pragma once
#include "./Command/Command.h"
class LeftMoveCommand :	public Command
{
public:
	LeftMoveCommand() {};
	~LeftMoveCommand() {};
public:
	virtual void execute(GameActor& actor);
};
