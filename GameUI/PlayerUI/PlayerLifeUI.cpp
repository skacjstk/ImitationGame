#include "ImitationGame/framework.h"
#include "GameUI/PlayerUI/PlayerUI.h"
#include "PlayerLifeUI.h"

PlayerLifeUI::PlayerLifeUI()
{
	SetActive(false);
	wstring strImage = IMAGE_FOLDER; strImage += L"UI/PlayerLifeBase 1.png";
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	imageBase_ = new Texture(strImage, strShader);
	strImage = IMAGE_FOLDER; strImage += L"UI/PlayerLifeBack.png";
	imageBack_ = new Texture(strImage, strShader);
}

PlayerLifeUI::~PlayerLifeUI()
{
	SAFE_DELETE(imageBase_);
	SAFE_DELETE(imageBack_);
}

void PlayerLifeUI::Update(Matrix V, Matrix P)
{
	imageBack_->Update(V, P);
	imageBase_->Update(V, P);
}

void PlayerLifeUI::Render()
{
	if (IsActive() == false)
		return;
	imageBack_->Render();
	imageBase_->Render();
}

void PlayerLifeUI::Reset()
{
	imageBase_->SetScale(GetScale());
	imageBack_->SetScale(GetScale());
	imageBase_->SetPosition(GetPosition());
	imageBack_->SetPosition(GetPosition());
	SetActive(true);
}
