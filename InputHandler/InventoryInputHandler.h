#pragma once	
#include "InputHandler/InputHandler.h"
class InventoryInputHandler : public InputHandler
{
public:
	InventoryInputHandler();
	vector<Command*> handleInput() override;
	virtual ~InventoryInputHandler() {};
public:	// ¸í·É bind ¿ë 
	void BindActorInput();
private:
	Command* buttonLCONTROL_;
};
