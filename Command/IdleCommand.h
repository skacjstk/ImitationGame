#pragma once
#include "./Command/Command.h"
class IdleCommand :	public Command
{
public:
	IdleCommand() {};
	~IdleCommand() {};
public:
	virtual void execute(GameActor& actor);
};
