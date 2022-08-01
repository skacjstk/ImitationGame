#include "./ImitationGame/framework.h"
#include "./Object/Line.h"
#include "./Object/Enemy/SkelDog.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "Object/Enemy/LittleGhost.h"
#include "Scenes/DungeonFactory.h"
#include "Room.h"

Room::Room() : roomType_(RoomType::DISABLE)
{
	GroundLine_ = new Line();
	CeilingLine_ = new Line();
	PlatformLine_ = new Line();
}

Room::Room(RoomType type) : roomType_(type)
{
	GroundLine_ = new Line();
	CeilingLine_ = new Line();
	PlatformLine_ = new Line();
}

Room::Room(RoomType type, int x, int y, int floor) : roomType_(type), myIndex{ x,y }, currentFloor_(floor)
{
	GroundLine_ = new Line();
	CeilingLine_ = new Line();
	PlatformLine_ = new Line();
}

Room::~Room()
{
	SAFE_DELETE(terrainImage_);
}

void Room::Update()
{
	Matrix V, P;
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();

	terrainImage_->Update(V, P);
	GroundLine_->Update(V, P);
	CeilingLine_->Update(V, P);
	PlatformLine_->Update(V, P);
	OBJECTMANAGER->UpdateAll(V, P);

	for (const auto& iter : roomObjects) {
		iter.second->Update(V,P);
	}

}

void Room::Render()
{
	terrainImage_->Render();
	GroundLine_->Render();
	CeilingLine_->Render();
	PlatformLine_->Render();
	for (const auto& iter : roomObjects) {
		iter.second->Render();
	}
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
	
//	DungeonFactory::GenerateDungeon(this);

	// 오브젝트 매니저를 통한 배치가 아닌, 자신만의 오브젝트 맵이 있음.
	GetRoomObjectData();

	value = true;
	return value;
}

void Room::LoadObjFile(string strFileName)
{
	FILE* fp;

	// Texture값 Setting

	fp = fopen(strFileName.c_str(), "r");
	if (fp == NULL)
		return;

	// Ascii File인 경우 1 Line씩 데이터를 read
	while (1)
	{
		char buf[1024];
		int  X, Y;
		char imgBuf[200];
		int  nOrder = 0;
		int objectType = 0;
		Vector2 Offset = Vector2(0.0f, 0.0f);
		Vector2 OffsetSize = Vector2(0.0f, 0.0f);
		Vector2 scale = Vector2(1.0f, 1.0f);
		int   nFlip = 0;
		float nAngle = 0.0f;
		int success = 0;

		int objNum = 0;	// 일렬번호? 같은거

		if (!fgets(buf, 1024, fp))
			break;
		//	buf[strlen(buf) - 1] = '\0'; // NewLine데이터 삭제
		if (strlen(buf) < 10)
			continue;
		if (strstr(buf, "#"))
			continue;

		//	0   10 -500.00 -200.00 | . / Image//Floor/Wall7.png|       1 0 0 100 112 
		float FX, FY;

		success = sscanf(buf, "%d %d %f %f", &X, &Y, &FX, &FY);
		char* p = strstr(buf, "|");
		p++;
		strcpy(imgBuf, p);
		p = strstr(imgBuf, "|");
		*p = '\0';

		string   str = imgBuf;
		wstring  str2;
		str2.assign(str.begin(), str.end());

		p = strstr(buf, "|");
		p++;

		p = strstr(p, "|");
		p++;

		Vector2 offset;
		Vector2 offsetSize;
		success = sscanf_s(p, "%d %f %f %f %f %f %f %f %d", &nOrder, &offset.x, &offset.y, &offsetSize.x, &offsetSize.y, &scale.x, &scale.y, &nAngle, &objectType);
		//_s는 %d, %f같은 기본사양은 크기지정 안해줘도 됨.


		// imgBuf의 내용은 SRVMANAGER에 저장
		wstring strImage = str2;
		// Texture에 저장

		// 6 * 10
		wstring strMap = to_wstring(X) + L"," + to_wstring(Y);

		// 이때쯤 이미 Wscale이 곱해져 있어야 함
		Vector2 position = Vector2(FX * TRNMANAGER->TerrainMagnification_.x, FY * TRNMANAGER->TerrainMagnification_.y);

		GameObject* pObject;

		wstring objName = L"";	// 해당 오브젝트 이름
		int objIndex = 0;	// 해당 오브젝트의 index 번호: 더미

		pObject = objectDB.FindActor(objectType,objName,objIndex);

		roomObjects.insert(std::make_pair(objName + to_wstring(objIndex), pObject));

		pObject->SetPosition(position);
		pObject->SetScale(scale);
		pObject->SetRotation(0.0f, 0.0f, nAngle);

	//	pTile->SetOrder(m_pTexture, nOrder, strImage, offset, offsetSize, nFlip, nAngle, scale, objectType);
	}
	fclose(fp);
}
// 파일 이름을 가지고 terrainImage_ 를 초기화해줌
void Room::SetTerrainImage(wstring strFileName, Vector2 pos)
{
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	terrainImage_ = new Texture(strFileName, strShader);
	terrainImage_->SetScale(TRNMANAGER->TerrainMagnification_.x, TRNMANAGER->TerrainMagnification_.y);
	terrainImage_->SetPosition(pos);
}

GameObject* Room::FindObject(wstring objName)
{
	GameObject* pObject = nullptr;

	if (roomObjects.count(objName))
		pObject = roomObjects[objName];

	return pObject;
}

void Room::Reset()
{
	for (const auto &iter : roomObjects) {
		iter.second->Reset();
	}
}

void Room::GetRoomObjectData()
{
	//1. myIndex를 따라 자신의 오브젝트 txt 파일을 가져감.

	string fileName = ROOM_FOLDER;
	fileName += to_string(currentFloor_) + "F/";
	fileName += to_string(myIndex[0]) + to_string(myIndex[1]) + "ObjectDesc.txt";
	LoadObjFile(fileName);

	//2. switch 문을 통해 해당 객체의 오브젝트 타입에 따라 new 이후 자신만의 맵에 넣어주며, 위치, 크기에 대한 초기화를 수행함


	//2-5. 크기에 대한 초기화는 각 옵젝에 기본적으로 있음( 하드코딩 했었잖아 )
}

