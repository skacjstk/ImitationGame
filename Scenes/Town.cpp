#include "./ImitationGame/framework.h"
#include "Town.h"

Town::Town()
{
	m_strSceneName = SCENEMANAGER->sceneNames_[1];	// Town
	wstring strImage = IMAGE_FOLDER; strImage += L"Intro/Title.png";
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	wstring strHoverImage;
	titleImage_ = new Texture(strImage, strShader);
	titleImage_->SetPosition(0.0f, 200.0f);
}

Town::~Town()
{
	SAFE_DELETE(titleImage_);
}

void Town::Update()
{
	Matrix V, P;
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();

	CAMERA->Update(V, P);

	titleImage_->Update(V, P);
}

void Town::Render()
{
	titleImage_->Render();
}

void Town::ChangeScene()
{
}

