#pragma once
#include "./Command/Command.h"
class UnderJumpCommand :	public Command
{
public:
	UnderJumpCommand() {};
	~UnderJumpCommand() {};
public:
	virtual void execute(GameActor& actor);
};
