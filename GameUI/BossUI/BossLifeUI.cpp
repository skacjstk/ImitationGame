#include "ImitationGame/framework.h"
#include "BossLifeUI.h"

BossLifeUI::BossLifeUI()
{
	SetActive(false);
	wstring strImage = IMAGE_FOLDER; strImage += L"UI/BossLifeBase.png";
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	imageBase_ = new Texture(strImage, strShader);
	strImage = IMAGE_FOLDER; strImage += L"UI/BossLifeBack.png";	//74 x 16
	imageBack_ = new Texture(strImage, strShader);
	strImage = IMAGE_FOLDER; strImage += L"UI/BossLifeBar.png";	//49 x 10
	lifeBar_ = new Texture(strImage, strShader);
	strImage = IMAGE_FOLDER; strImage += L"UI/BossSkellPortrait.png";
	bossPortrait_ = new Texture(strImage, strShader);
}

BossLifeUI::~BossLifeUI()
{
	SAFE_DELETE(imageBase_);
	SAFE_DELETE(imageBack_);
	SAFE_DELETE(lifeBar_);
	SAFE_DELETE(bossPortrait_);
}

void BossLifeUI::Update(Matrix V, Matrix P)
{
}

void BossLifeUI::Update()
{
	if (IsActive() == false)
		return;
	Matrix abV, P;
	abV = CAMERA->GetAbsoluteViewMatrix();
	P = CAMERA->GetProjectionMatrix();

	imageBack_->Update(abV, P);
	lifeBar_->Update(abV, P);
	bossPortrait_->Update(abV, P);
	imageBase_->Update(abV, P);
}

void BossLifeUI::Render()
{
	if (IsActive() == false)
		return;
	imageBack_->Render();
	lifeBar_->Render();
	bossPortrait_->Render();
	imageBase_->Render();
}
void BossLifeUI::Reset()
{
	SetScale(5.0f * WSCALEX, 5.0f * WSCALEY);

	// 위치 보정용으로 사용할 변수, 결국 Reset()을 두번 
	Vector2 bossLifeRealSize = imageBase_->GetTextureRealSize();
	// dash는 9 x 4 카운트 그거가 크기임. ( 이거 자꾸 크기 안맞으면 학원에서 9 x 8 로 바꿀 수 있음 )
	SetPosition(0.0f, (-1000 * WSCALEY * 0.5f) + bossLifeRealSize.y * 0.5f);

	imageBase_->SetScale(GetScale());
	imageBack_->SetScale(GetScale());
	bossPortrait_->SetScale(GetScale());
	imageBase_->SetPosition(GetPosition());
	imageBack_->SetPosition(GetPosition());
	modifyScale = lifeBarBaseScale = GetScale();

	lifeBar_->SetScale(lifeBarBaseScale);

	float hpInversePosToPivot = lifeBar_->GetTextureRealSize().x * 0.5f;
	Vector2 tempRS = imageBase_->GetTextureRealSize();

	lifeBar_->_pivot = Vector3(-hpInversePosToPivot, 0.0f, 0.0f);
	lifeBar_->SetPosition(GetPosition().x + (tempRS.x * 0.076f) - hpInversePosToPivot, GetPosition().y);
	// lifeBar 폰트 위치 및 크기 조정.
	bossPortrait_->SetPosition(GetPosition());
	bossPortrait_->SetPosition(-hpInversePosToPivot * 1.02f, GetPosition().y);
	// lifeBar 폰트 위치 및 크기 조정.

	float temp = ((((WSCALEX + WSCALEY) / 2) - 1.0f) / 2) + 1.0f;
	font_size = font_size * temp;
	hpDescPos = lifeBar_->GetPosition();
	hpDescPos.x += -lifeBar_->_pivot.x;
	hpDescPos.y += font_size * 0.5f;
	SetActive(true);
}

void BossLifeUI::UpdateLifeBar(int currentHp, int maxHP)
{
	modifyScale.x = (float)currentHp / (float)maxHP;
	modifyScale.x *= lifeBarBaseScale.x;
	lifeBar_->SetScale(modifyScale);
	hpDesc = to_wstring(currentHp) + L" / " + to_wstring(maxHP);
	lifeBar_->_pivot = Vector3(-lifeBar_->GetTextureRealSize().x * 0.5f, 0.0f, 0.0f);
	Update();
}
