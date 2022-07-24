#pragma once
#include "GameUI/BaseUI.h"
#include "GameUI/PlayerUI/DashUI.h"
#include "GameUI/PlayerUI/PlayerLifeUI.h"
#include "GameUI/PlayerUI/EquippedWeaponUI.h"
class PlayerUI : public BaseUI
{
public:
	class PlayerLifeUI* playerLifeUI_ = nullptr;
	class EquippedWeaponUI* equippedWeaponUI_ = nullptr;
	class DashUI* dashUI_ = nullptr;

public: // ������ & �Ҹ���
	PlayerUI();
	~PlayerUI();
public:
	void Update(Matrix V, Matrix P) override;
	void Update();	// ���� MainUI �� ������ �ִ� ��� UI ������Ʈ�ϱ�.
	void Render() override;	
	void Reset() override;
public:
//	void SetActive(bool active);	// ����: ���� ���߿� ��г��� �غ���. 
	//���� Ư����. false�� ��� Render()�� �Լ� �����Ͱ� �����Լ��� �̵���.

};
