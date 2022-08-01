#include "./ImitationGame/framework.h"
#include "./Object/Player.h"
#include "./Object/NPC.h"
#include "./Object/NPCPool.h"
#include "./Object/NPC/Dungeon.h"
#include "./Physics/Collider.h"
#include "./Object/Line.h"
#include "./Render/Graphic2D.h"
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

	strImage = IMAGE_FOLDER; strImage += L"Town/TownBG_Day_3.png";
	mountain_ = new Texture(strImage, strShader);
	mountain_->SetScale(backGround_->GetScale());
	strImage = IMAGE_FOLDER; strImage += L"Town/TownLayer_Day_3.png";
	forest_ = new Texture(strImage, strShader);
	forest_->SetScale(backGround_->GetScale());


	m_pGroundLine = new Line();
	m_pCeilingLine = new Line();
	m_pPlatformLine = new Line();
	npc = NPCPOOL->GetNPC(0);

	strImage = IMAGE_FOLDER; strImage += L"Town/townBase.png";
	townTerrain_ = new Texture(strImage, strShader);
	townTerrain_->SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);


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
	cameraMoveX = CAMERA->cameraAfter.x - CAMERA->cameraBefore.x;
	cameraMoveY = CAMERA->cameraAfter.y - CAMERA->cameraBefore.y;
	if(fabsf(cameraMoveX) < 200.0f * WSCALEX && fabsf(cameraMoveY) < 200.0f * WSCALEY)
	mountain_->ModifyPosition(cameraMoveX * 0.8f, cameraMoveY * 0.6f);
	forest_->ModifyPosition(cameraMoveX * 0.6f,cameraMoveY * 0.4f);
	CAMERA->Update(V, P);
	backGround_->Update(abV, P);
	townTerrain_->Update(V, P);
	mountain_->Update(V, P);
	forest_->Update(V, P);
	m_pGroundLine->Update(V, P);
	npc->Update(V, P);
}

void Town::Render()
{
	backGround_->Render();
//	TRNMANAGER->Render();	// 최적화 실패
	mountain_->Render();
	forest_->Render();
	townTerrain_->Render();
	m_pGroundLine->Render();
	npc->Render();
	OBJECTMANAGER->RenderAll();
}

void Town::ChangeScene()
{
//	Graphic2D* graphic = new Graphic2D();
//
//	graphic->GdiPlusStartUp();
//	graphic->Merge();
//	graphic->GdiPlusShutDown();

	SetActive(true);
	Audio->Play("Town", 1.0f);
	// 임시 코드
	tempPlayer = new Player();
	tempPlayer->SetPosition(2250.0f * WSCALEX, -1725.0f * WSCALEY);
	OBJECTMANAGER->AddObject("player" , tempPlayer);
	OBJECTMANAGER->AddObjectStrings("player");	// 이것도 추가해줘야 함.
	tempPlayer->Reset();

	mountain_->SetPosition(2250.0f * WSCALEX, -1825.0f * WSCALEY);
	forest_->SetPosition(2250.0f * WSCALEX, -1225.0f * WSCALEY);
	
	npc->SetPosition(1885.5f * WSCALEX, -2085.0f * WSCALEY);
	npc->Reset();
	CAMERA->SetObject(tempPlayer);
	CAMERA->SetCenterXLock(0);

	TRNMANAGER->SetSceneMap("../MapData/GraphicData/Town", true);	// TerrainMagnification을 가져오기 위해 필요함. terrainMag만 가져오는 함수 추가 예정
	TRNMANAGER->TerrainMagnification_.x *= WSCALEX;
	TRNMANAGER->TerrainMagnification_.y *= WSCALEY;
	m_pGroundLine->LoadLine("../MapData/LineData/testcoord.txt");
	// 카메라의 최대, 최소값 잡아주기 (MoMoDora Camera 참조) Next0701

	townTerrain_->SetPosition(2110.0f * WSCALEX, -1025.0f * WSCALEY);

	// 메인 맵 이미지 위치가 잡히고 나서 좌우 정해주기
	Vector2 leftDown = townTerrain_->GetPosition();
	Vector2 rightTop = leftDown;

	leftDown -= townTerrain_->GetTextureRealSize() * 0.5f;
	rightTop += townTerrain_->GetTextureRealSize() * 0.5f;
	// 이후 X값만 윈도우 너비만큼 보정해주면 된다.
	leftDown.x += MAIN->GetWidth() * 0.5f;
	rightTop.x -= MAIN->GetWidth() * 0.5f;

	// y값도 보정해주긴 해야 한다. // 그건 인던에서 하자.
	CAMERA->SetCornerLeft(leftDown);
	CAMERA->SetCornerRight(rightTop);

}

void Town::ExitScene()
{
	Audio->Stop("Town");
}

