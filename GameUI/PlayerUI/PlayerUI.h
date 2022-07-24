#pragma once
#include "GameUI/BaseUI.h"
class PlayerUI : public BaseUI
{
public:
	class PlayerLifeUI* playerLifeUI_ = nullptr;
	class EquippedWeaponUI* equippedWeaponUI_ = nullptr;
	class DashUI* dashUI_ = nullptr;

public: // 생성자 & 소멸자
	PlayerUI();
	~PlayerUI();
public:
	void Update(Matrix V, Matrix P) override;
	void Update();	// 현재 MainUI 가 가지고 있는 모든 UI 업데이트하기.
	void Render() override;	
	void Reset() override;
public:
//	void SetActive(bool active);	// 더미: 조금 나중에 결론나면 해보자. 
	//조금 특수함. false일 경우 Render()의 함수 포인터가 더미함수로 이동함.

};
