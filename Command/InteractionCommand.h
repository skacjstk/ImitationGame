#pragma once
#include "./Command/Command.h"
class InteractionCommand :	public Command
{
public:
	InteractionCommand() {};
	~InteractionCommand() {};
public:
	virtual void execute(GameActor& actor);
};
