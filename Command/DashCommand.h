#pragma once
#include "./Command/Command.h"
class DashCommand : public Command
{
public:
	virtual ~DashCommand() {};
	virtual void execute(GameActor& actor);
};