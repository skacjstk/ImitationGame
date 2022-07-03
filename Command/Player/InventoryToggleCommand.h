#pragma once
#include "./Command/Command.h"
class InventoryToggleCommand :	public Command
{
public:
	InventoryToggleCommand() {};
	~InventoryToggleCommand() {};
public:
	virtual void execute(GameActor& actor);
};
