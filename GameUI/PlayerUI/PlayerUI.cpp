#include "ImitationGame/framework.h"
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
	printf("PlayerUI::Update(V,P) 를 업데이트 하고 있습니다. Update() 를 호출하세요.\n");
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
// 매 프레임마다 Render 되며, 위치는 SCENEMANAGER에 반복문 이후
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

	// 위치 보정용으로 사용할 변수, 결국 Reset()을 두번 
	Vector2 playerLifeRealSize = playerLifeUI_->GetTextureRealSize();
	Vector2 WeaponUIRealSize = equippedWeaponUI_->GetTextureRealSize();
	Vector2 dashUIRealSize = dashUI_->GetTextureRealSize();
	// dash는 9 x 4 카운트 그거가 크기임. ( 이거 자꾸 크기 안맞으면 학원에서 9 x 8 로 바꿀 수 있음 )

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