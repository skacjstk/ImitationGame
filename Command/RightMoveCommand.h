#pragma once
#include "./Command/Command.h"
class RightMoveCommand :	public Command
{
public:
	RightMoveCommand() {};
	~RightMoveCommand() {};
public:
	virtual void execute(GameActor& actor);
};
