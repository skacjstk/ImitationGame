#pragma once
class Dungeon : public NPC
{
public:		// 공개 인스턴스 변수
	enum DungeonState {
		EAT = 0,
		INGURGITATE	= 1,
		HIDE = 2
	};
	// 던그리드 캐릭터 변경할때 사용할 것
private:	// 비공개 인스턴스 변수
	DungeonState _currentState = DungeonState::HIDE;
	class Player* ppPlayer = { nullptr, };
public:
	Dungeon();
	~Dungeon();

	void Update(Matrix V, Matrix P) override;
	void Render() override;
	void Reset() override;
	void Communicate() override;
	void CheckPlayer();
};