#include "./ImitationGame/framework.h"
#include "./Object/Player.h"
#include "./Physics/Collider.h"
#include "./Object/Line.h"
#include "./Object/Weapons/MeleeWeapons/ShortSword.h"	// Next: 지울 예정( 무기와 Player 관계에 대한 고민 )
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

	// 테스트코드
	tempPlayer_ = new Player();
	tempLine_ = new Line();

//	CAMERA->SetObject(tempPlayer_);
	tempLine_->AddLine(-500.0f, -200.0f, 500.0f, -200.0f);
	tempLine_->EndLine();
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
	// Next: 충돌법좀 물어봅시다.
//	tempLine_->InterSectionLine(tempPlayer_->GetCollider(),);

	CAMERA->Update(V, P);
	backGround_->Update(V, P);
	tempPlayer_->Update(V, P);
	tempLine_->Update(V, P);
}

void Town::Render()
{
	backGround_->Render();
	tempPlayer_->Render();
	tempLine_->Render();
}

void Town::ChangeScene()
{
	SetActive(true);
	// 임시 코드
	// 카메라의 최대, 최소값 잡아주기 (MoMoDora Camera 참조) Next0701
}

