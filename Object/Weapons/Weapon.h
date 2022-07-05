#pragma once
#include "./Object/Item.h"
class Weapon : public Item
{
public:
	enum class WeaponType
	{
		NONE = 0,
		MELEE= 1,
		RANGED = 2,
		SUB = 4
	};
	enum class HandedType
	{
		NONE = 0,
		ONEHANDED = 1,
		TWOHANDED = 2,
	};
protected:
//	Texture* pTexture_ = nullptr;	// Item�� frontImage_ �� �̵�
	Animation* weapon_ = nullptr;
	GameActor** owner_ = nullptr;
	Vector2 LeftHandPoint_[2];	// �� ���⸦ �������� ��, PlayerHand �� ���־�� �� ��ġ, (�� �� �ؼ� �ι�(�޼�)
	Vector2 RightHandPoint_[2];	// ������ ��ġ ����Ʈ
	float minDamage_ = 1.0f;
	float maxDamage_ = 1.0f;
	float attackSpeed_ = 1.0f;
	enum class WeaponType weaponType_ = WeaponType::NONE;
	enum class HandedType handedType_ = HandedType::NONE;
public:
	Weapon() {};
	~Weapon();
public:
	virtual void Update(Matrix V, Matrix P) = 0;  // ���������Լ�
	virtual void Render() = 0;                    // ���������Լ�
	virtual void Reset() = 0;                     // ���������Լ�
	virtual void Fire() = 0;	// ��� ����� fire �Լ��� ����� �����ؾ� �Ѵ�.
public:
	//Setter
	void SetOwner(GameActor** owner) { owner_ = owner; }
	void SetWeaponPosition(Vector2 position) { weapon_->SetPosition(position); }
	void SetWeaponRotation(float x, float y, float z) { weapon_->SetRotation(x, y, z); }
	void SetWeaponScale(Vector2 scale) { weapon_->SetScale(scale); }
	// Getter
	auto GetWeaponPosition() { return weapon_->GetPosition(); }
	auto GetWeaponRotation() { return weapon_->GetRotation(); }
	auto GetWeaponScale()	{ return weapon_->GetScale(); }
};
