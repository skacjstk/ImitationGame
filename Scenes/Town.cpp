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

//	CAMERA->SetObject(tempPlayer_);

	wstring introBGM = AUDIO_FOLDER;
	introBGM += L"0.Town.wav";
	Audio->AddSound("Town", introBGM, true);
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
	// Next: �浹��/ ��ü ������ ����.

	CAMERA->Update(V, P);
	backGround_->Update(V, P);
	tempPlayer_->Update(V, P);
}

void Town::Render()
{
	backGround_->Render();
	tempPlayer_->Render();
}

void Town::ChangeScene()
{
	SetActive(true);
	Audio->Play("Town", 1.0f);
	// �ӽ� �ڵ�
	// ī�޶��� �ִ�, �ּҰ� ����ֱ� (MoMoDora Camera ����) Next0701
}

void Town::ExitScene()
{
	Audio->Stop("Town");
}

void Town::TestGroundCheck()
{
	/*
	vector<class Line*> lines;
	Scene* currentScene = SCENEMANAGER->GetCurrentScene();
	//Scene ��ȣ(���ڿ�) �� ���� �ٴ� ���� ��������	
	//printf("%s\n", currentScene->GetName().c_str()); 
	if (currentScene->GetName() == "S05_MoModoraDemo1")
	{
		S05_MoModoraDemo1* Scene1 = (S05_MoModoraDemo1*)currentScene;
		lines = Scene1->GetLines();
	}
	if (currentScene->GetName() == "S06_MoModoraDemo2")
	{
		S06_MoModoraDemo2* Scene2 = (S06_MoModoraDemo2*)currentScene;
		lines = Scene2->GetLines();
	}
	Vector2 size = animation->GetTexture()->GetRealTextureSize();
	Vector2 vertical = Vector2(position.x, position.y - (size.y * 0.5f));
	Vector2 result;
	bool Check = false;
	_isGround = false;
	for (UINT i = 0; i < lines.size(); ++i) {
		Line* line = lines.at(i);
		for (UINT j = 0; j < line->GetSize() - 1; ++j) {
			Vector2 start = line->GetPosition(j);
			Vector2 end = line->GetPosition(j + 1);
			Check = Collider::InterSectionLine(start, end, position, vertical, result);
			if (Check)
				break;
		}//end for
		if (Check)
			break;
	}//end for

	if (Check)
	{
		_isGround = true;
		position.y = result.y + size.y * 0.5f;
	}
	*/
}

