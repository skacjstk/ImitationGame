#pragma once
class NPC : public GameActor
{
public:		// ���� �ν��Ͻ� ����
	enum class StateNPC
	{
		STUCK = 0,
		RESCUED,
		OTHER,
		OTHER2
	};
	// ���׸��� ĳ���� �����Ҷ� ����� ��
private:	// ����� �ν��Ͻ� ����
	Animation* _animation = nullptr;
	int codeNPC_ = -1;
	StateNPC _currentState = StateNPC::RESCUED;
	StateNPC _beforeState = StateNPC::RESCUED;
public:
	NPC(int NPCID);
	~NPC();
public: // Getter
	int GetCodeNPC() { return codeNPC_; }
public:	// Setter
	void SetCodeNPC(int code) { codeNPC_ = code; }
};