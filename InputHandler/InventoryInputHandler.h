#pragma once
#include "InputHandler/InputHandler.h"	// 왜 Player쪽은 괜찮은데 얘만 이걸 헤더에 선언해야 할까?
class InventoryInputHandler : public InputHandler
{
public:
	InventoryInputHandler();
	vector<class Command*> handleInput() override;
	virtual ~InventoryInputHandler() {};
public:	// 명령 bind 용 
	void BindActorInput();
private:
	Command* buttonLCONTROL_;
};
