#pragma once
class Weapon : public GameObject
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
	Texture* pTexture_ = nullptr;
	Vector2 LeftHandPoint_[2];	// 이 무기를 장착했을 때, PlayerHand 가 가있어야 할 위치, (휙 휙 해서 두번(왼손)
	Vector2 RightHandPoint_[2];	// 오른손 위치 포인트
	float minDamage_ = 1.0f;
	float maxDamage_ = 1.0f;
	string weaponText_ = "";
	float attackSpeed_ = 1.0f;
	enum class WeaponType weaponType_ = WeaponType::NONE;
	enum class HandedType handedType_ = HandedType::NONE;
public:
	Weapon() {};
	~Weapon() {};
public:
	virtual void Update(Matrix V, Matrix P) = 0;  // 순수가상함수
	virtual void Render() = 0;                    // 순수가상함수
	virtual void Reset() = 0;                     // 순수가상함수
};
