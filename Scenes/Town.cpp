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
	
//	CAMERA->SetObject(tempPlayer_);

	wstring introBGM = AUDIO_FOLDER;
	introBGM += L"0.Town.wav";
	Audio->AddSound("Town", introBGM, true);

	m_pGroundLine = new Line();

}

Town::~Town()
{
	SAFE_DELETE(backGround_);
}

void Town::Update()
{
	Matrix V, P, abV;
	V = CAMERA->GetViewMatrix();
	abV = CAMERA->GetAbsoluteViewMatrix();
	P = CAMERA->GetProjectionMatrix();
	// Next: 충돌법
	OBJECTMANAGER->UpdateAll(V, P);		

	CAMERA->Update(V, P);
	backGround_->Update(abV, P);
	m_pGroundLine->Update(V, P);
//	TRNMANAGER->Update(V, P);	// 현재 TRN Update는 사실상 더미
}

void Town::Render()
{
	backGround_->Render();
	TRNMANAGER->Render();
	OBJECTMANAGER->RenderAll();
	m_pGroundLine->Render();
}

void Town::ChangeScene()
{
	SetActive(true);
	Audio->Play("Town", 1.0f);
	// 임시 코드
	Player* tempPlayer = new Player();
	tempPlayer->SetPosition(3000.0f, -1300.0f);
	OBJECTMANAGER->AddObject("player" , tempPlayer);
	OBJECTMANAGER->AddObjectStrings("player");	// 이것도 추가해줘야 함.
	CAMERA->SetObject(tempPlayer);
	CAMERA->SetCenterXLock(0);
	TRNMANAGER->SetSceneMap("../MapData/GraphicData/Town");
	TRNMANAGER->TerrainMagnification_.x *= WSCALEX;
	TRNMANAGER->TerrainMagnification_.y *= WSCALEY;
	m_pGroundLine->LoadLine("../MapData/LineData/testcoord.txt");
	// 카메라의 최대, 최소값 잡아주기 (MoMoDora Camera 참조) Next0701
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
	//Scene 번호(문자열) 에 따라서 바닥 정보 가져오기	
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

