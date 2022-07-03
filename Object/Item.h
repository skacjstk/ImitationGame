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
	~Item();
public:
	virtual void Update(Matrix V, Matrix P) = 0;  // 순수가상함수
	void FrontImageUpdate(Matrix V, Matrix P);		// 이미지만 업데이트하기
	virtual void Render() = 0;                    // 순수가상함수
	void FrontImageRender();					// 이미지만 렌더하기
	virtual void Reset() = 0;                     // 순수가상함수
};
