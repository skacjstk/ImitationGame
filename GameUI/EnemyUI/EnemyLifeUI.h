#pragma once
#include "GameUI/BaseUI.h"
#include "./Render/Texture.h"
class EnemyLifeUI : public BaseUI
{
	class Texture* imageBase_ = nullptr;
	class Texture* imageBack_ = nullptr;
	Texture* lifeBar_ = nullptr;
	Vector2 lifeBarBaseScale = Vector2(0.0f, 0.0f);	
	Vector2 modifyScale = Vector2(0.0f, 0.0f);	// x scale�� ������ ü�¹ٸ� ǥ���� ��
	// ü�� �ٿ� ǥ���� ����
	wstring hpDesc = L"";
	Vector2 hpDescPos = Vector2(0.0f, 0.0f);
	float font_size = 45.0f;
public: // ������ & �Ҹ���
	EnemyLifeUI();
	~EnemyLifeUI();
public:	// ���� �ν��Ͻ� ���� 
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;
	void  Reset() override;
public:
	Vector2 GetTextureRealSize() { return imageBase_->GetTextureRealSize(); }	// dashUI ��ġ ������
	void  UpdateLifeBar(int currentHp, int maxHP);
};
