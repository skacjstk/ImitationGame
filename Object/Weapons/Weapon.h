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
	struct weaponTransform {
		Vector2 scale = Vector2(1.0f, 1.0f);
		Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f);
		Vector2 position = Vector2(0.0f, 0.0f);
	};
protected:
//	Texture* pTexture_ = nullptr;	// Item에 frontImage_ 로 이동
	Animation* weapon_ = nullptr;
	GameActor** owner_ = nullptr;
	Vector2 LeftHandPoint_[2];	// 이 무기를 장착했을 때, PlayerHand 가 가있어야 할 위치, (휙 휙 해서 두번(왼손)
	Vector2 RightHandPoint_[2];	// 오른손 위치 포인트
	float minDamage_ = 1.0f;
	float maxDamage_ = 1.0f;
	float attackSpeed_ = 1.0f;
	enum class WeaponType weaponType_ = WeaponType::NONE;
	enum class HandedType handedType_ = HandedType::NONE;
	struct weaponTransform weaponTransform_;
public:
	Weapon() {};
	~Weapon();
public:
	virtual void Update(Matrix V, Matrix P) = 0;  // 순수가상함수
	virtual void Render() = 0;                    // 순수가상함수
	virtual void Reset() = 0;                     // 순수가상함수
	virtual void Fire() = 0;	// 모든 무기는 fire 함수를 상속해 구현해야 한다.
public:
	//Setter
	void SetOwner(GameActor** owner) { owner_ = owner; }
	void SetWeaponPosition(Vector2 position) { weaponTransform_.position = position; }
	void SetWeaponRotation(float x, float y, float z) { weaponTransform_.rotation = Vector3(x, y, z); }
	void SetWeaponRotation(Vector3 rotation) { weaponTransform_.rotation = rotation; }
	void SetWeaponScale(Vector2 scale) { weaponTransform_.scale = scale; }
	void SetWeaponScale(float x, float y) { weaponTransform_.scale = Vector2(x,y); }
	// Getter
	auto GetWeaponPosition() { return weaponTransform_.position; }
	auto GetWeaponRotation() { return weaponTransform_.rotation; }
	auto GetWeaponScale()	{ return weaponTransform_.scale; }
	auto GetWeaponTransform() { return weaponTransform_; }
};
