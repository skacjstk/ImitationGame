#include "ImitationGame/framework.h"
#include "GameUI/PlayerUI/PlayerUI.h"
#include "PlayerLifeUI.h"

PlayerLifeUI::PlayerLifeUI()
{
	SetActive(false);
	wstring strImage = IMAGE_FOLDER; strImage += L"UI/PlayerLifeBase 1.png";
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	imageBase_ = new Texture(strImage, strShader);
	strImage = IMAGE_FOLDER; strImage += L"UI/PlayerLifeBack.png";	//74 x 16
	imageBack_ = new Texture(strImage, strShader);
	strImage = IMAGE_FOLDER; strImage += L"UI/PlayerLifeBar.png";	//49 x 10
	lifeBar_ = new Texture(strImage, strShader);
}

PlayerLifeUI::~PlayerLifeUI()
{
	SAFE_DELETE(imageBase_);
	SAFE_DELETE(imageBack_);
	SAFE_DELETE(lifeBar_);
}

void PlayerLifeUI::Update(Matrix V, Matrix P)
{
	imageBack_->Update(V, P);
	lifeBar_->Update(V, P);
	imageBase_->Update(V, P);
}

void PlayerLifeUI::Render()
{
	if (IsActive() == false)
		return;
	imageBack_->Render();
	lifeBar_->Render();
	imageBase_->Render();
	DirectWrite::RenderTextCenter(hpDesc, hpDescPos.x, hpDescPos.y, 255, 255, 255, font_size);
}
void PlayerLifeUI::Reset()
{
	imageBase_->SetScale(GetScale());
	imageBack_->SetScale(GetScale());
	imageBase_->SetPosition(GetPosition());
	imageBack_->SetPosition(GetPosition());
	modifyScale = lifeBarBaseScale = GetScale();

	lifeBar_->SetScale(lifeBarBaseScale);

	float hpInversePosToPivot = lifeBar_->GetTextureRealSize().x * 0.5f;
	Vector2 tempRS = imageBase_->GetTextureRealSize();

	lifeBar_->_pivot = Vector3(-hpInversePosToPivot, 0.0f, 0.0f);
	lifeBar_->SetPosition(GetPosition().x + (tempRS.x * 0.128f) - hpInversePosToPivot, GetPosition().y);
	// lifeBar 폰트 위치 및 크기 조정.

	float temp = ((((WSCALEX + WSCALEY) / 2) - 1.0f) / 2) + 1.0f;
	font_size = font_size * temp;
	hpDescPos = lifeBar_->GetPosition();
	hpDescPos.x += -lifeBar_->_pivot.x;
	hpDescPos.y += font_size * 0.5f;
	SetActive(true);
}

void PlayerLifeUI::UpdateLifeBar(int currentHp, int maxHP)
{
	modifyScale.x = (float)currentHp / (float)maxHP;
	modifyScale.x *= lifeBarBaseScale.x;
	lifeBar_->SetScale(modifyScale);
	hpDesc = to_wstring(currentHp) + L" / " + to_wstring(maxHP);
	lifeBar_->_pivot = Vector3(-lifeBar_->GetTextureRealSize().x * 0.5f, 0.0f, 0.0f);
}
