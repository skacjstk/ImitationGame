#include "ImitationGame/framework.h"
#include "GameUI/PlayerUI/DashUI.h"
#include "GameUI/PlayerUI/PlayerLifeUI.h"
#include "GameUI/PlayerUI/EquippedWeaponUI.h"
#include "PlayerUI.h"

PlayerUI::PlayerUI()
{
	playerLifeUI_ = new PlayerLifeUI();
	equippedWeaponUI_ = new EquippedWeaponUI();
	dashUI_ = new DashUI();
}

PlayerUI::~PlayerUI()
{
	SAFE_DELETE(playerLifeUI_);
	SAFE_DELETE(equippedWeaponUI_);
	SAFE_DELETE(dashUI_);
}

void PlayerUI::Update(Matrix V, Matrix P)
{
	printf("PlayerUI::Update(V,P) �� ������Ʈ �ϰ� �ֽ��ϴ�. Update() �� ȣ���ϼ���.\n");
}

void PlayerUI::Update()
{
	Matrix abV, P;
	abV = CAMERA->GetAbsoluteViewMatrix();
	P = CAMERA->GetProjectionMatrix();
	playerLifeUI_->Update(abV, P);
	equippedWeaponUI_->Update(abV, P);
	dashUI_->Update(abV, P);
}
// �� �����Ӹ��� Render �Ǹ�, ��ġ�� SCENEMANAGER�� �ݺ��� ����
void PlayerUI::Render()
{
	playerLifeUI_->Render();
	equippedWeaponUI_->Render();
	dashUI_->Render();
}

void PlayerUI::Reset()
{
	playerLifeUI_->SetScale(5.0f * WSCALEX, 5.0f * WSCALEY);
	equippedWeaponUI_->SetScale(5.0f * WSCALEX, 5.0f * WSCALEY);
	dashUI_->SetScale(5.0f * WSCALEX, 5.0f * WSCALEY);

	playerLifeUI_->Reset();
	equippedWeaponUI_->Reset();
	dashUI_->Reset();

	// ��ġ ���������� ����� ����, �ᱹ Reset()�� �ι� 
	Vector2 playerLifeRealSize = playerLifeUI_->GetTextureRealSize();
	Vector2 WeaponUIRealSize = equippedWeaponUI_->GetTextureRealSize();
	Vector2 dashUIRealSize = dashUI_->GetTextureRealSize();
	// dash�� 9 x 4 ī��Ʈ �װŰ� ũ����. ( �̰� �ڲ� ũ�� �ȸ����� �п����� 9 x 8 �� �ٲ� �� ���� )

	playerLifeUI_->SetPosition((-940.0f * WSCALEX) + playerLifeRealSize.x * 0.5f, 
		(520.0f * WSCALEY) - playerLifeRealSize.y * 0.5f);
	equippedWeaponUI_->SetPosition((940.0f * WSCALEX) - WeaponUIRealSize.x * 0.5f,
		(-520.0f * WSCALEY) + WeaponUIRealSize.y * 0.5f);
	dashUI_->SetPosition((-940.0f * WSCALEX) + dashUIRealSize.x * 0.5f,
		(440.0f * WSCALEY) - playerLifeRealSize.y * 0.5f);		

	playerLifeUI_->Reset();
	equippedWeaponUI_->Reset();
	dashUI_->Reset();
}							

/*
playerLifeUI_ 
equippedWeaponUI_
dashUI_
*/