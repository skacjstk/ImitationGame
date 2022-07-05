#include "./ImitationGame/framework.h"
#include "./Object/Player.h"
#include "./Physics/Collider.h"
#include "./Object/Line.h"
#include "./Object/Weapons/MeleeWeapons/ShortSword.h"	// Next: ���� ����( ����� Player ���迡 ���� ��� )
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

	// �׽�Ʈ�ڵ�
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
	// Next: �浹���� ����ô�.
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
	// �ӽ� �ڵ�
	// ī�޶��� �ִ�, �ּҰ� ����ֱ� (MoMoDora Camera ����) Next0701
}

