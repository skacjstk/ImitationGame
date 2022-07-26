#include "./ImitationGame/framework.h"
#include "./Object/Line.h"
#include "./Object/Enemy/SkelDog.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "Object/Enemy/LittleGhost.h"
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
	PlatformLine_->Update(V, P);
	OBJECTMANAGER->UpdateAll(V, P);
}

void Room::Render()
{
	terrainImage_->Render();
	GroundLine_->Render();
	CeilingLine_->Render();
	PlatformLine_->Render();
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
	
	GetRoomObjectData();
	// �ӽ� �� ��ü
	SkelDog* obj = new SkelDog();
	obj->Reset();
	OBJECTMANAGER->AddObject("SkelDog1", obj);
	OBJECTMANAGER->AddObjectStrings("SkelDog1");

//	BigWhiteSkel* obj2 = new BigWhiteSkel();
//	obj2->Reset();
//	OBJECTMANAGER->AddObject("BWS1", obj2);
//	OBJECTMANAGER->AddObjectStrings("BWS1");
//
//	LittleGhost* obj3 = new LittleGhost();
//	obj3->Reset();
//	OBJECTMANAGER->AddObject("LG1", obj3);
//	OBJECTMANAGER->AddObjectStrings("LG1");

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
	PlatformLine_ = new Line();
	PlatformLine_->LoadLine("../MapData/LineData/testMapPlatformLine.txt");

	// ������Ʈ �Ŵ����� ���� ��ġ�� �ƴ�, �ڽŸ��� ������Ʈ ���� ����.
	value = true;
	return value;
}

void Room::GetRoomObjectData()
{
	//1. myIndex�� ���� �ڽ��� ������Ʈ txt ������ ������.
	//2. switch ���� ���� �ش� ��ü�� ������Ʈ Ÿ�Կ� ���� new ���� �ڽŸ��� �ʿ� �־��ָ�, ��ġ, ũ�⿡ ���� �ʱ�ȭ�� ������
	//2-5. ũ�⿡ ���� �ʱ�ȭ�� �� ������ �⺻������ ����( �ϵ��ڵ� �߾��ݾ� )
}

