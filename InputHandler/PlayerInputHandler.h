#pragma once
class PlayerInputHandler : public InputHandler
{
public:
	PlayerInputHandler();
	vector<class Command*> handleInput() override;
	virtual ~PlayerInputHandler() {};
public:	// ¸í·É bind ¿ë 
	void BindActorInput();
private:
	Command* buttonA_;
	Command* buttonD_;
	Command* buttonS_;
	Command* buttonS_SPACE;
	Command* buttonW_;
	Command* buttonV_;
	Command* idleCommand_;
	Command* mouse0_;
};
