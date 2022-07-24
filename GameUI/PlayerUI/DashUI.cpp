#include "ImitationGame/framework.h"
#include "GameUI/PlayerUI/PlayerUI.h"
#include "DashUI.h"

DashUI::DashUI()
{
	SetActive(false);
	wstring strImage = IMAGE_FOLDER; strImage += L"UI/DashBaseLeftEnd.png";
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	left_ = new Texture(strImage, strShader);

	strImage = IMAGE_FOLDER; strImage += L"UI/DashBaseRightEnd.png";
	right_ = new Texture(strImage, strShader);

	strImage = IMAGE_FOLDER; strImage += L"UI/DashBase.png";
	base_ = new Texture(strImage, strShader);

	strImage = IMAGE_FOLDER; strImage += L"UI/DashCount.png";
	count_ = new Texture(strImage, strShader);
}

DashUI::~DashUI()
{
	SAFE_DELETE(left_);
	SAFE_DELETE(right_);
	SAFE_DELETE(base_);
	SAFE_DELETE(count_);
}

void DashUI::Update(Matrix V, Matrix P)
{
	left_->Update(V, P);
	right_->Update(V, P);
	base_->Update(V, P);
	count_->Update(V, P);
}

void DashUI::Render()
{
	if (IsActive() == false)
		return;
	left_->Render();
	right_->Render();
	base_->Render();
	count_->Render();
}

void DashUI::Reset()
{
	left_->SetScale(GetScale());
	right_->SetScale(GetScale());
	base_->SetScale(GetScale());
	count_->SetScale(GetScale());

	left_->SetPosition(GetPosition());
	right_->SetPosition(GetPosition());
	base_->SetPosition(GetPosition());
	count_->SetPosition(GetPosition());
	SetActive(true);
}
/*
	left_
	right_
	base_
	count_
*/