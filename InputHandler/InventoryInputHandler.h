#pragma once
#include "InputHandler/InputHandler.h"	// �� Player���� �������� �길 �̰� ����� �����ؾ� �ұ�?
class InventoryInputHandler : public InputHandler
{
public:
	InventoryInputHandler();
	vector<class Command*> handleInput() override;
	virtual ~InventoryInputHandler() {};
public:	// ��� bind �� 
	void BindActorInput();
private:
	Command* buttonLCONTROL_;
};
