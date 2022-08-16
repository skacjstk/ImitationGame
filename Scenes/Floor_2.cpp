#include "./ImitationGame/framework.h"
#include "Scenes/Room.h"
#include "Object/Player.h"
#include "DungeonFactory.h"
#include "Floor_2.h"

Floor_2::Floor_2()
{
	SetActive(false);
	m_strSceneName = "Floor_2";
	m_strNextSceneName = "Floor_3";
}

Floor_2::~Floor_2()
{
	SAFE_DELETE(backGround_);
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 4; ++j) {
			SAFE_DELETE(roomData_[i][j]);
		}
	}
}

void Floor_2::Update()
{
	roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->Update();
}

void Floor_2::Render()
{
	roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->Render();
}
// ��� ���Ⱑ ����.
void Floor_2::ChangeScene()
{
	// ������ ���� ���� ���, ���� ũ�� Room ������ �о����� ����
	ApplyStartRoom();	// 0 0�� start room
//	ApplyEndRoom();		// 2�� endRoom�� ���߿� ������: Next
	EnterRoom();	// ���۷� �����ڸ��� ����
	Audio->Play("Floor_2BGM", 1.0f);

	// ������� OtherRoom �� EndRoom �����
	// 10 20 21 �� �׳� ��� �� ( NPC�Ĵ簰���� �ϴ� ���� )
	thread t(bind(&Floor_2::ApplyOtherRoom, this));
	t.detach();
}

void Floor_2::ExitScene()
{
}

void Floor_2::MoveRoom(int x, int y)
{
	roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->SetActive(false);
	currentActiveRoom_[0] += x;
	currentActiveRoom_[1] += y;
	
	int xy[2] = { currentActiveRoom_[0], currentActiveRoom_[1] };
	Vector2 setPlayerPos = Vector2(0.0f, 0.0f);
	switch (x)
	{
	case 1:	// Right
		// Left Stele ��ġ ��������
		setPlayerPos = roomData_[xy[0]][xy[1]]->GetPosOfDirection(3);
		break;
	case -1:	// Left
		// Right Stele ��ġ ��������
		setPlayerPos = roomData_[xy[0]][xy[1]]->GetPosOfDirection(1);
		break;
	}
	switch (y)
	{
	case 1:	// Down
		// UP Stele ��ġ ��������
		setPlayerPos = roomData_[xy[0]][xy[1]]->GetPosOfDirection(2);
		break;
	case -1:	// Up
		// Down Stele ��ġ ��������
		setPlayerPos = roomData_[xy[0]][xy[1]]->GetPosOfDirection(0);
		break;
	}
	Player* player = (Player*)OBJECTMANAGER->FindObject("player");
	player->SetPosition(setPlayerPos);
	EnterRoom();

	// ������ ��ǥ�� Player���� �����Ű��
}

void Floor_2::ReadStartRoomMinimal()
{
	TRNMANAGER->SetSceneMap("test", true);
	TRNMANAGER->TerrainMagnification_.x *= WSCALEX;
	TRNMANAGER->TerrainMagnification_.y *= WSCALEY;
}

void Floor_2::ApplyStartRoom()
{
	ReadStartRoomMinimal();
	// myIndex�� �� ���� �־���� ��.
	roomData_[0][0] = new Room(Room::RoomType::START, 0, 0, currentFloor_);
	// txt ���� �о� ������ �־��ֱ�
	ReadRoomData(0, 0);
	currentActiveRoom_[0] = 0;
	currentActiveRoom_[1] = 0;

	// �׽�Ʈ�ڵ�
	GameActor* temp = (GameActor*)roomData_[0][0]->FindObject(L"Door1");
	// �׽�Ʈ�ڵ�
	GameActor* player = (GameActor*)OBJECTMANAGER->FindObject("player");

	player->SetPosition(temp->GetPosition());

}

void Floor_2::ApplyEndRoom()
{
	roomData_[2][0] = new Room(Room::RoomType::END,2,0, currentFloor_);
	// txt ���� �о� ������ �־��ֱ�
	ReadRoomData(2, 0);
}

void Floor_2::ApplyOtherRoom()
{
	int xy[1] = { 10 };
	int x, y;
	for (int i = 0; i < _countof(xy); ++i) {
		x = xy[i] / 10;
		y = xy[i] % 10;
		roomData_[x][y] = new Room(Room::RoomType::NORMAL,x,y, currentFloor_);
		roomData_[x][y]->myIndex[0] = x;
		roomData_[x][y]->myIndex[1] = y;
		// txt ���� �о� ������ �־��ֱ�
		ReadRoomData(x, y);
	}
}
// �� ���� �̹���, ���� Line ���� �б�
void Floor_2::ReadRoomData(int x, int y)
{
	Room* tempRoom = roomData_[x][y];	// ���ϰ� �Ϸ��� ������ 

	string coord = to_string(x)+ to_string(y);
	wstring coord2 = to_wstring(x) + to_wstring(y);
	// terrain �̹��� ��ġ
	wstring filePath = ROOM_FOLDER_W;
	filePath += to_wstring(currentFloor_) + L"F/" + coord2 + L"Terrain.png";	// ������ġ
	
	Vector2 pos = Vector2(0.0f, 0.0f);
	pos = TRNMANAGER->GetTileSize() * -0.5f;
	pos.x *= TRNMANAGER->TerrainMagnification_.x;
	pos.y *= -TRNMANAGER->TerrainMagnification_.y; 

	tempRoom->SetTerrainImage(filePath, pos);
	ReadLines(tempRoom, coord);
	
	tempRoom->InitializeRoom();	
	objectDB.InitializeNumOneArray();	// ���� �ʱ�ȭ ���� ������ �ʱ�ȭ�ؾ���
}
// �� Line ���� �б�(�̶� �̹� mapscale�� Wscale�� ������ ����)
void Floor_2::ReadLines(Room* tempRoom, string coord)
{	
	string filePath = "../RoomData/" + to_string(currentFloor_) + "F/" + coord + "GroundLine.txt";
	tempRoom->GroundLine_->LoadLine(filePath);
	filePath = "../RoomData/" + to_string(currentFloor_) + "F/" + coord + "CeilingLine.txt";
	tempRoom->CeilingLine_->LoadLine(filePath);
	filePath = "../RoomData/" + to_string(currentFloor_) + "F/" + coord + "PlatformLine.txt";
	tempRoom->PlatformLine_->LoadLine(filePath);
}

void Floor_2::EnterRoom()
{
	// ���� �˻�: ���� ������ �� clear ����
	if(roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->IsCleared() == false)
		roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->Reset();
	
	Texture* backImage = roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->terrainImage_;

	// ���� �� �̹��� ��ġ�� ������ ���� �¿� �����ֱ�
	Vector2 leftDown = backImage->GetPosition();
	Vector2 rightTop = leftDown;

	leftDown -= backImage->GetTextureRealSize() * 0.5f;
	rightTop += backImage->GetTextureRealSize() * 0.5f;
	// ���� X���� ������ �ʺ�ŭ �������ָ� �ȴ�.
	leftDown.x += MAIN->GetWidth() * 0.5f;
	leftDown.y += MAIN->GetHeight() * 0.5f;

	rightTop.x -= MAIN->GetWidth() * 0.5f;
	rightTop.y -= MAIN->GetHeight() * 0.5f;

	// y���� �������ֱ� �ؾ� �Ѵ�. // �װ� �δ����� ����.
	CAMERA->SetCornerLeft(leftDown);
	CAMERA->SetCornerRight(rightTop);
}

////////���� ������ ��������
/*
void Floor_2::GenerateRoom()
{
	// ���� ��ġ�� �� ��ġ ����
	int SX;
	int SY;
	int EX;
	int EY;
	srand(time(NULL));
	while (true)
	{
		SX = rand() % 8;
		SY = rand() % 4;
		EX = rand() % 8;
		EY = rand() % 4;
		if (abs(SX - EX) + abs(SY - EY) > 6) {	// 6ĭ���� �ָ� ����
			SetSRER(SX, SY, EX, EY);
			currentActiveRoom_[0] = SX;
			currentActiveRoom_[1] = SY;
			break;	// ����
		}
	}
	// �� �� ���� ������ �̾����� ��.
	// ������ 2���� �迭�̰� ����갰���͵� ������ ��â�� �˰����� �ʿ䰡 ����.
	// �� �������� SX EY ������ ������.
	ConnectSRER(SX, SY, EX, EY);
}
// SX, SY�� ���� ��ġ��, EX, EY�� �� ��ġ�� �����Ѵ�. 
// out of range �˻簡 ������ ����.
void Floor_2::SetSRER(int& SX, int& SY, int& EX, int& EY)
{
	// 0 7, 0 3 ���� 
	tempRoom[SX][SY] = Room::RoomType::START;	// ���� ��
	tempRoom[EX][EY] = Room::RoomType::END;	// �� ��

	// ��¥ �� �����
	roomData_[SX][SY] = new Room(Room::RoomType::START);
	roomData_[EX][EY] = new Room(Room::RoomType::END);
}
// ���� ��ġ�� �� ��ġ�� �̾��ִ� �Լ�
// SX, SY�� �����ϴ� ���� ������ �ʿ��ϴٸ� ���� �Ҵ��س��� �Ѵ�.
void Floor_2::ConnectSRER(int& SX, int& SY, int& EX, int& EY)
{

	bool possibilityX = true;
	bool possibilityY = true;
	while (!(SX == EX) || !(SY == EY))
	{
		possibilityX = true;
		possibilityY = true;	// ���� ��ġ ���� �� ��ġ�� �����¿� ��� �Ӽ��� ������ �ִ��� �˻� ( d = distance )
		int dX = EX - SX;	// ������ left, ����� right
		int dY = SY - EY;    // ����� UP, ������ Down
		float diceX = 0.0f;
		float diceY = 0.0f;
		if (dX == 0)	// X�� Ȯ����� ����.
		{
			possibilityX = false;
		}
		if (dY == 0) // Y�� Ȯ����� ����.
		{
			possibilityY = false;
		}
		// �Ѵ� ���϶��� �� ������ Ȯ��������
		if (possibilityX == true || possibilityY == true) {
			// ��� ���ϸ� 0 �� �ǰ���?
			// ������ ������ �ȵǳ�?
		//	std::random_device rd;
		//	std::default_random_engine eng(rd());		
		//	std::uniform_real_distribution<int> dist(0, (abs(dX)) + (abs(dY)) - 1);

			int randn = rand() % ((abs(dX)) + (abs(dY)));
			printf("�Ÿ� ����:%d\n", (abs(dX)) + (abs(dY)));
			printf("��µ� random ��:%d\n", randn);
			if (randn - (abs(dY)) < 0) {
				// DY ��÷
				// Y�� �ø��ų� ����
				if (dY < 0)
					++SY;
				else
					--SY;
			}
			else {
				// DX ��÷
				// X�� �ø��ų� ����
				if (dX > 0)
					++SX;
				else
					--SX;
			}
			// ���� �ݿ� ���� �˻� ( ��ġ�� end�� �ȵ� )
			if (tempRoom[SX][SY] != (int)Room::RoomType::END) {
				printf("\n������ġ: %d %d\n", SX, SY);
				// ���� ��ġ�� �ƴϴ� �ش� ��ġ�� NORMAL�� ����
				tempRoom[SX][SY] = Room::RoomType::NORMAL;
				// ��¥ �� �����
				roomData_[SX][SY] = new Room(Room::RoomType::NORMAL);
			}
			else // �� ����Ȱ���. ���۰� ���� �����ϱ�				
				break;

		} // end if
	}//end while
}
*/