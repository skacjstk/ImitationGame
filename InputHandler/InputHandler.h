#pragma once
class InputHandler
{
public:
	virtual vector<class Command*> handleInput() = 0;
	~InputHandler() {};
};
