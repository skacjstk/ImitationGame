#pragma once
class Item : public GameObject
{
public:
	enum class RARITY {
		COMMON = 1,
		UNCOMMON = 2,
		RARE = 4,
		LEGENDARY = 8
	};
protected:
	Texture* frontImage_ = nullptr;
	string ItemText_ = "";
public:
	Item() {};
	~Item() {};
public:
	virtual void Update(Matrix V, Matrix P) = 0;  // ���������Լ�
	virtual void Render() = 0;                    // ���������Լ�
	virtual void Reset() = 0;                     // ���������Լ�
};
