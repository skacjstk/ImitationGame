#pragma once
#include "./Command/Command.h"
class JumpCommand :	public Command
{
public:
	JumpCommand() {};
	~JumpCommand() {};
public:
	virtual void execute(GameActor& actor);
};
