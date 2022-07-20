#pragma once
class GameActor : public GameObject
{
public:
	struct ActorData
	{
		int HP = 0;
		int ImmuneTime = 0;
	};
protected:
	struct ActorData actorData_;
	class Collider* pCollider_ = nullptr;
	class Animation* _animation = nullptr;
	float gravity_ = 0.0f;	// Actor�鿡�� ����Ǵ� �� �����Ӹ��� �Ʒ��� �������� ��ȭ��
	bool isGround_ = false;	// Actor �ڽ��� ���� ��Ҵ��� Ȯ���� ����
	bool isConflicted_ = false;	// �� ���� �ٸ� ���� �ε����� ��, ����� ���� ���� ���
	bool isFall = false;
	bool isJump = false;
	bool beforeGround_ = false;
	// Actor�� �����ð��� ������ (player ���� �˴� 0, �̹��������� �������� �����ڸ� �����Ӹ��� -1 �ϰԵ�)
	int ImmuneFrame_ = 0;	
	int maxImmuneFrame_ = 0;	// �ʱ�ȭ ���� ��õ��Actor Reset. actorData.ImmuneTime * GetFrame
public:
	GameActor() {};
	~GameActor(){};
	// Command ���
	virtual void GroundCheck();
	void GravityUpdate();
	virtual void Attacked();
	virtual void LeftMove() {};
	virtual void RightMove() {};
	virtual void Jump() {};
	virtual void Move() {};	// �Ļ���: 0630 ���� Left,Right���� ������ �������� Move�� ȣ���ϴ� ����. 
	virtual void Idle() {};		// IDLE�� Attack�� �� �־�� ��.
	virtual void Attack() {};
	virtual void InventoryToggle() {};
	virtual void EquipChange(int index) {};	// �������� ��ü
	virtual void ItemDragAndDrop() {};	// ������ �巡�׾� ��� ( ���ü�� ���� )
	virtual void SwapHandFocus() {};
	virtual void UpdateHandedWeapon() {};	// �κ��丮���� ������ ���� ���ſ�
	// Getter
	int GetImmuneFrame() { return ImmuneFrame_; }
	// Setter
	void SetImmuneFrame(int frame) { ImmuneFrame_ = frame; }
public:
	class Collider* GetCollider() { return pCollider_; }
};
