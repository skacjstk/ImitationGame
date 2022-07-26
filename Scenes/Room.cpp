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
// 사실 오류검출 방법이 없어...
bool Room::InitializeRoom()
{
	bool value = false;
	
	// 테스트룸은 시작부분에서
	if (roomType_ != RoomType::START)
		return false;

	DungeonFactory::GenerateDungeon(this);
	
	GetRoomObjectData();
	// 임시 적 객체
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
	// 최종적으로 이게 되어야함.

	GroundLine_ = new Line();
	GroundLine_->LoadLine("../MapData/LineData/testMapLine.txt");
	CeilingLine_ = new Line();
	CeilingLine_->LoadLine("../MapData/LineData/testMapCelingLine.txt");
	PlatformLine_ = new Line();
	PlatformLine_->LoadLine("../MapData/LineData/testMapPlatformLine.txt");

	// 오브젝트 매니저를 통한 배치가 아닌, 자신만의 오브젝트 맵이 있음.
	value = true;
	return value;
}

void Room::GetRoomObjectData()
{
	//1. myIndex를 따라 자신의 오브젝트 txt 파일을 가져감.
	//2. switch 문을 통해 해당 객체의 오브젝트 타입에 따라 new 이후 자신만의 맵에 넣어주며, 위치, 크기에 대한 초기화를 수행함
	//2-5. 크기에 대한 초기화는 각 옵젝에 기본적으로 있음( 하드코딩 했었잖아 )
}

