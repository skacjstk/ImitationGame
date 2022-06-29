#include "./ImitationGame/framework.h"
#include "./Object/Player.h"
#include "./Physics/Collider.h"
#include "Town.h"

Town::Town()
{
	SetActive(false);
	m_strSceneName = "Town";	// Town
	wstring strImage = IMAGE_FOLDER; strImage += L"Town/Map_Tiles.png";
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	
	// 317, 181
	backGround_ = new Texture(strImage, strShader,0, 521, 318, 702);
	backGround_->SetScale(6.0377358f * WSCALEX, 5.9668508f * WSCALEY);
	backGround_->SetPosition(0.0f, 0.0f);
	printf("%f %f\n", backGround_->GetTextureRealSize().x, backGround_->GetTextureRealSize().y);

	tempPlayer_ = new Player();
	_tempFloor = new Collider();
	_tempFloor->SetScale(800.0f, 1.0f);
	_tempFloor->SetPosition(0.0f, -300.0f);
}

Town::~Town()
{
	SAFE_DELETE(backGround_);
	SAFE_DELETE(tempPlayer_);
}

void Town::Update()
{
	Matrix V, P;
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();

	CAMERA->Update(V, P);
	backGround_->Update(V, P);
	tempPlayer_->Update(V, P);
	_tempFloor->Update(V, P);
}

void Town::Render()
{
	backGround_->Render();
	tempPlayer_->Render();
	_tempFloor->Render();
}

void Town::ChangeScene()
{
	SetActive(true);
	// 임시 코드
}

