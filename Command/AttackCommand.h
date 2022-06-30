#pragma once
#include "./Command/Command.h"
class AttackCommand : public Command
{
public:
	virtual ~AttackCommand() {};
	virtual void execute(GameActor& actor);
};