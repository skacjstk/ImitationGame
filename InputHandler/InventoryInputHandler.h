#pragma once	
#include "InputHandler/InputHandler.h"
class InventoryInputHandler : public InputHandler
{
public:
	InventoryInputHandler();
	vector<Command*> handleInput() override;
	virtual ~InventoryInputHandler() {};
public:	// ��� bind �� 
	void BindActorInput();
private:
	Command* buttonLCONTROL_;
};
