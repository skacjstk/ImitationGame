#pragma once
class Dungeon : public NPC
{
public:		// ���� �ν��Ͻ� ����
	enum DungeonState {
		EAT = 0,
		INGURGITATE	= 1,
		HIDE = 2
	};
	// ���׸��� ĳ���� �����Ҷ� ����� ��
private:	// ����� �ν��Ͻ� ����
	DungeonState _currentState = DungeonState::HIDE;
	class Player* ppPlayer = { nullptr, };
	bool playSound_ = false;
	bool playSound2_ = false;
public:
	Dungeon();
	~Dungeon();

	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
	void Communicate() override;
	void CheckPlayer();
};