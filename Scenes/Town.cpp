#include "./ImitationGame/framework.h"
#include "./Object/Player.h"
#include "./Object/NPC.h"
#include "./Object/NPCPool.h"
#include "./Object/NPC/Dungeon.h"
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

	wstring introBGM = AUDIO_FOLDER;
	introBGM += L"0.Town.wav";
	Audio->AddSound("Town", introBGM, true);

	m_pGroundLine = new Line();

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
	// Next: �浹��
	OBJECTMANAGER->UpdateAll(V, P);		

	CAMERA->Update(V, P);
	backGround_->Update(abV, P);
	townTerrain_->Update(V, P);
	m_pGroundLine->Update(V, P);
	npc->Update(V, P);
}

void Town::Render()
{
	backGround_->Render();
//	TRNMANAGER->Render();	// ����ȭ ����
	townTerrain_->Render();
	m_pGroundLine->Render();
	npc->Render();
	OBJECTMANAGER->RenderAll();
}

void Town::ChangeScene()
{
	SetActive(true);
	Audio->Play("Town", 1.0f);
	// �ӽ� �ڵ�
	Player* tempPlayer = new Player();
	tempPlayer->SetPosition(3000.0f, -2300.0f);
	OBJECTMANAGER->AddObject("player" , tempPlayer);
	OBJECTMANAGER->AddObjectStrings("player");	// �̰͵� �߰������ ��.
	npc->SetPosition(1885.5f * WSCALEX, -2085.0f * WSCALEY);
	npc->Reset();
	CAMERA->SetObject(tempPlayer);
	CAMERA->SetCenterXLock(0);
	TRNMANAGER->SetSceneMap("../MapData/GraphicData/Town");
	TRNMANAGER->TerrainMagnification_.x *= WSCALEX;
	TRNMANAGER->TerrainMagnification_.y *= WSCALEY;
	m_pGroundLine->LoadLine("../MapData/LineData/testcoord.txt");
	// ī�޶��� �ִ�, �ּҰ� ����ֱ� (MoMoDora Camera ����) Next0701

	townTerrain_->SetPosition(2110.0f * WSCALEX, -1025.0f * WSCALEY);
}

void Town::ExitScene()
{
	Audio->Stop("Town");
}

