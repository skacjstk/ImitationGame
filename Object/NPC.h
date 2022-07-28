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
protected:	
	Animation* _animation = nullptr;
	int codeNPC_ = -1;	// DB에서 찾을때 사용할 것.
	StateNPC _currentState = StateNPC::RESCUED;
	StateNPC _beforeState = StateNPC::RESCUED;
public:
	NPC(int code);
	NPC() {};
	~NPC();

	virtual void Update(Matrix V, Matrix P) = 0;  // 순수가상함수
	virtual void Render() = 0;                    // 순수가상함수
	virtual void Reset() = 0;                    // 순수가상함수
	virtual void Communicate() = 0;

public: // Getter
	int GetCodeNPC() { return codeNPC_; }
public:	// Setter
	void SetCodeNPC(int code) { codeNPC_ = code; }
	void CreateNPC(int code);
};