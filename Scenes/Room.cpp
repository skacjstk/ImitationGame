#include "./ImitationGame/framework.h"
#include "./Object/Line.h"
#include "./Object/Enemy/SkelDog.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "Object/Enemy/LittleGhost.h"
#include "Scenes/DungeonFactory.h"
#include "Object/TileObject/Stele.h"
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

	if(isCleared_ == false)		// 클리어 한적 없을때만 오브젝트 데이터를 가져옴. (Reset 에도 있음)
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
	//	Vector2 position = Vector2(FX, FY);
		GameObject* pObject;
		Stele* pStele;
		wstring objName = L"";	// 해당 오브젝트 이름
		int objIndex = 1;	// 해당 오브젝트의 index 번호

		pObject = objectDB.FindActor(objectType,objName,objIndex);
		roomObjects.insert(std::make_pair(objName + to_wstring(objIndex), pObject));

		if (objectType < 100)		// >=100 부턴 NPC나 따른쪽이니까
			++currentMonsterNumber_;		// 몬스터 갯수를 하나씩 늘림
		pObject->SetPosition(position);
		pObject->SetScale(scale);
		pObject->SetRotation(0.0f, 0.0f, nAngle);

		if (objName.compare(L"Stele") == 0)	// 받아온게 Stele와 같으면
		{
			pStele = (Stele*)pObject;
			pStele->SetPath();
		}
		pObject = nullptr;
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

void Room::DecreaseCurrentMonsterNumber()
{
	currentMonsterNumber_ -= 1;
	if (currentMonsterNumber_ <= 0) {
		RoomClear();
	}
}

void Room::GetRoomObjectData()
{
	//1. myIndex를 따라 자신의 오브젝트 txt 파일을 가져감.

	string fileName = ROOM_FOLDER;
	fileName += to_string(currentFloor_) + "F/";
	fileName += to_string(myIndex[0]) + to_string(myIndex[1]) + "ObjectDesc.txt";
	LoadObjFile(fileName);

	//2. Stele 찾아서 sendPos
	Stele* tempStele = nullptr;
	//Stele를 1~4번까지 찾아본다. 이름짓는 순서를 생각해보면 못찾을 경우 취소.

	wstring steleName = L"";
	for (int i = 1; i <= posOfDirection.size(); ++i) {
		steleName = L"Stele" + to_wstring(i);	// 1 ~ 4 까지
		auto iter = roomObjects.find(steleName);
		if (iter != roomObjects.end())
		{
			tempStele = (Stele*)iter->second;
			Stele::StelePath ePath = tempStele->GetPath();
			PushDirection(ePath, tempStele->GetPosition());
		}
		else
			break;	// 탐색중단: 네이밍 순서 생각해봐.
	}
	// 최초 검사: 몹이 없으면 룸 clear 판정
	if (currentMonsterNumber_ == 0)
		RoomClear();
	//2-5. 크기에 대한 초기화는 각 옵젝에 기본적으로 있음( 하드코딩 했었잖아 )
}

void Room::PushDirection(Stele::StelePath ePath, Vector2 position)
{
	Vector2 comPosition = Vector2(0.0f, 0.0f);
	int index = -1;
	switch (ePath)
	{
	case Stele::StelePath::BOTTOM:
		comPosition = Vector2(0.0f, TRNMANAGER->GetTileSize().y * TRNMANAGER->TerrainMagnification_.y * 3.0f);	// 바텀은 좀 특별해서 3곱
		index = 0;
		break;
	case Stele::StelePath::RIGHT:
		comPosition = Vector2(TRNMANAGER->GetTileSize().x * TRNMANAGER->TerrainMagnification_.x * -2.0f, 0.0f);
		index = 1;
		break;
	case Stele::StelePath::TOP:
		comPosition = Vector2(0.0f, TRNMANAGER->GetTileSize().y * TRNMANAGER->TerrainMagnification_.y * -2.0f);
		index = 2;
		break;
	case Stele::StelePath::LEFT:
		comPosition = Vector2(TRNMANAGER->GetTileSize().x * TRNMANAGER->TerrainMagnification_.x * 2.0f, 0.0f);
		index = 3;
		break;
	}
	if (index != -1) {
		canDirection[index] = true;
		comPosition += position;
		posOfDirection[index] = comPosition;	// 틀린건 어째서인지 Stele의 좌표. 아니, 10의 좌표가 다 이상함.
	//	printf("결국 생성된 위치: %f %f\n", comPosition.x, comPosition.y);
	}
}

void Room::RoomClear()
{
	isCleared_ = true;
	// 자신의 Room이 가진 Stele 들에게 방이 클리어되었다는 신호를 줌
	Stele* tempStele = nullptr;
	wstring steleName = L"";
	for (int i = 1; i <= posOfDirection.size(); ++i) {
		steleName = L"Stele" + to_wstring(i);	// 1 ~ 4 까지
		auto iter = roomObjects.find(steleName);
		if (iter != roomObjects.end())
		{
			tempStele = (Stele*)iter->second;
			tempStele->roomCleared_ = true;
		}
		else
			break;	// 탐색중단: 네이밍 순서 생각해봐.
	}
}

