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
protected:	
	Animation* _animation = nullptr;
	int codeNPC_ = -1;	// DB���� ã���� ����� ��.
	StateNPC _currentState = StateNPC::RESCUED;
	StateNPC _beforeState = StateNPC::RESCUED;
public:
	NPC(int code);
	NPC() {};
	~NPC();

	virtual void Update(Matrix V, Matrix P) = 0;  // ���������Լ�
	virtual void Render() = 0;                    // ���������Լ�
	virtual void Reset() = 0;                    // ���������Լ�
	virtual void Communicate() = 0;

public: // Getter
	int GetCodeNPC() { return codeNPC_; }
public:	// Setter
	void SetCodeNPC(int code) { codeNPC_ = code; }
	void CreateNPC(int code);
};