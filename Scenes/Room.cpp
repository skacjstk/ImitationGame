#include "./ImitationGame/framework.h"
#include "./Object/Line.h"
#include "./Object/Enemy/SkelDog.h"
#include "Scenes/DungeonFactory.h"
#include "Room.h"

Room::~Room()
{
	SAFE_DELETE(terrainImage_);
}

void Room::Update()
{
//	printf("%d %d\n", myIndex[0], myIndex[1]);
	Matrix V, P;
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();

	terrainImage_->Update(V, P);
	GroundLine_->Update(V, P);
	CeilingLine_->Update(V, P);
	OBJECTMANAGER->UpdateAll(V, P);
}

void Room::Render()
{
	terrainImage_->Render();
	GroundLine_->Render();
	CeilingLine_->Render();
	OBJECTMANAGER->RenderAll();
}

void Room::ChangeScene()
{
}

void Room::ExitScene()
{
}
// ��� �������� ����� ����...
bool Room::InitializeRoom()
{
	bool value = false;
	
	// �׽�Ʈ���� ���ۺκп���
	if (roomType_ != RoomType::START)
		return false;

	DungeonFactory::GenerateDungeon(this);
	
	// �ӽ� �˰� ��ü
	SkelDog* obj = new SkelDog();
	obj->Reset();
	OBJECTMANAGER->AddObject("SkelDog", obj);
	OBJECTMANAGER->AddObjectStrings("SkelDog");

	//terrainImage_ 
	wstring shader = SHADER_FOLDER;	shader += L"TextureColor.hlsl";
	wstring strImage = L"../MapData/GraphicData/testMap.png";

	terrainImage_ = new Texture(strImage, shader);

	terrainImage_->SetPosition(2345.5f * WSCALEX, -2355.0f * WSCALEY);
	terrainImage_->SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
	// ���������� �̰� �Ǿ����.

	GroundLine_ = new Line();
	GroundLine_->LoadLine("../MapData/LineData/testMapLine.txt");
	CeilingLine_ = new Line();
	CeilingLine_->LoadLine("../MapData/LineData/testMapCelingLine.txt");
	value = true;
	return value;
}

