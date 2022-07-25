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
}

void DashUI::Render()
{
	if (IsActive() == false)
		return;
	left_->Render();
	right_->Render();
	DashRender();
}

void DashUI::Reset()
{
	left_->SetScale(GetScale());
	right_->SetScale(GetScale());
	base_->SetScale(GetScale());
	count_->SetScale(GetScale());

	left_->SetPosition(GetPosition());

	baseGap = base_->GetTextureRealSize();

	rightPos = Vector2(baseGap.x * (maxDashCount) + (baseGap.x * 0.2f), rightPos.y);
		rightPos += GetPosition();
	right_->SetPosition(rightPos);

	firstBasePos = GetPosition();
	firstBasePos.x += baseGap.x * 0.6f;

	base_->SetPosition(firstBasePos);
	count_->SetPosition(GetPosition());
	SetActive(true);
}
// 최대 대시횟수가 변경될 때 호출하면 됩니다.
void DashUI::InitializeDashCount(int maxDashCount)
{
	maxDashCount = currentDashCount = maxDashCount;
}
void DashUI::Dash()
{
	DecreaseDashCount(1);
}
void DashUI::DashRender()
{
	Matrix abV, P;
	abV = CAMERA->GetAbsoluteViewMatrix();
	P = CAMERA->GetProjectionMatrix();
	// base는 max 수만큼 UpRe 하고,
	for (int i = 0; i < maxDashCount; ++i) {
		base_->SetPosition(firstBasePos.x + (baseGap.x * i), firstBasePos.y);
		base_->Update(abV, P);
		base_->Render();
	}
	// count는 currnet 수만큼 UpRe 하자
	for (int i = 0; i < currentDashCount; ++i) {
		count_->SetPosition(firstBasePos.x + (baseGap.x * i), firstBasePos.y);
		count_->Update(abV, P);
		count_->Render();
	}
}
void DashUI::IncreaseDashCount(int amount)
{
	currentDashCount = min(maxDashCount, currentDashCount + amount);
}
void DashUI::DecreaseDashCount(int amount)
{
	currentDashCount -= amount;
}
/*
	left_
	right_
	base_
	count_
*/