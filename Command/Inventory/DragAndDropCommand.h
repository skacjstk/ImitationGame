#pragma once
#include "./Command/Command.h"
class DragAndDropCommand :	public Command
{
public:
	DragAndDropCommand() {};
	~DragAndDropCommand() {};
public:
	virtual void execute(GameActor& actor);
};
