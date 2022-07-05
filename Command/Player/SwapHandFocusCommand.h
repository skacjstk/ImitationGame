#pragma once
#include "./Command/Command.h"
class SwapHandFocusCommand : public Command
{
public:
	virtual ~SwapHandFocusCommand() {};
	virtual void execute(GameActor& actor);
};