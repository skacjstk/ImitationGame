#include "./ImitationGame/framework.h"
#include "TestScene.h"

TestScene::TestScene()
{
	wstring strImage = IMAGE_FOLDER; strImage += L"testTitle.png";
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	_texture = new Texture(strImage, strShader);
}

TestScene::~TestScene()
{
	SAFE_DELETE(_texture);
	SAFE_DELETE(_animation);
}

void TestScene::Update()
{
	Matrix V, P;
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();

	CAMERA->Update(V, P);
	_texture->Update(V, P);
}

void TestScene::Render()
{
	_texture->Render();
	CAMERA->Render();
}

void TestScene::ChangeScene()
{
}
