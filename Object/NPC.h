#pragma once
class NPC : public GameActor
{
public:		// 공개 인스턴스 변수
	enum class StateNPC
	{
		STUCK = 0,
		RESCUED,
		OTHER,
		OTHER2
	};
	// 던그리드 캐릭터 변경할때 사용할 것
private:	// 비공개 인스턴스 변수
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