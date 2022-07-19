#include "./ImitationGame/framework.h"
#include "Scenes/Room.h"
#include "DungeonFactory.h"
#include "Floor_1.h"

Floor_1::Floor_1()
{
	SetActive(false);
	m_strSceneName = "Floor_1";
}

Floor_1::~Floor_1()
{
	SAFE_DELETE(backGround_);
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 4; ++j) {
			SAFE_DELETE(roomData_[i][j]);
		}
	}
}

void Floor_1::Update()
{
	Matrix abV, P;
	abV = CAMERA->GetAbsoluteViewMatrix();
	P = CAMERA->GetProjectionMatrix();
//	backGround_->Update(abV, P);	// 그 돌무대기 배경
	roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->Update();
}

void Floor_1::Render()
{
//	backGround_->Render();
	roomData_[currentActiveRoom_[0]][currentActiveRoom_[1]]->Render();
}

void Floor_1::ChangeScene()
{
	// 1. 방 생성
	// 2. 시작 위치와 끝 위치 연결
	GenerateRoom();
	// 3. 방 유효성 확인 (  시작과 끝이 연결되었는가? )
	ConnectRoom(currentActiveRoom_[0], currentActiveRoom_[1]);
	// DFS 를 통해 시작과 끝이 연결되어있는지 확인. ( 0 이 아니면 일단 연결된 것 )
	// 4. 특수 방 (1개만 있는) 지정(시작, 보스입구 빼고)
	// 나중에 하자
	// 5. 유효성 검사( 모든 맵을 탐색할 수 있는지? )
	// BFS 모든 방이 시작점을 기준으로 탐색 가능한지 검사
	// 6. 이를 기반으로 맵 생성
	GenerateRoomData();
	// 6-1. 맵 생성시 필요한 것 메모
	/*
		방의 크기, 상하좌우 포탈의 위치를 기록한 생성 데이터
		동적 생성된 Gate ( 입자 나오는 포탈 )를 포함한 오브젝트 데이터
		위의 Gate 영향받아 벽 부분이 동적 생성된 완성된 방의 이미지 데이터
		이미지 데이터와 동기화된 Line 데이터 ( 일단 정적 생성으로 )
		room 간의 연결 정보를 담나?
		시작 위치값을 넣어주기
	*/

}

void Floor_1::ExitScene()
{
}

void Floor_1::GenerateRoom()
{
	// 시작 위치와 끝 위치 지정
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
		if (abs(SX - EX) + abs(SY - EY) > 6) {	// 6칸보다 멀면 성공
			SetSRER(SX, SY, EX, EY);
			currentActiveRoom_[0] = SX;
			currentActiveRoom_[1] = SY;
			break;	// 성공
		}
	}
	// 이 두 룸은 무조건 이어져야 함.
	// 어차피 2차원 배열이고 비용계산같은것도 없으니 거창한 알고리즘은 필요가 없음.
	// 이 시점에서 SX EY 변수가 오염됨.
	ConnectSRER(SX, SY, EX, EY);
}
// SX, SY를 시작 위치로, EX, EY를 끝 위치로 지정한다. 
// out of range 검사가 없으니 주의.
void Floor_1::SetSRER(int& SX, int& SY, int& EX, int& EY)
{
	// 0 7, 0 3 사이 
	tempRoom[SX][SY] = Room::RoomType::START;	// 시작 룸
	tempRoom[EX][EY] = Room::RoomType::END;	// 끝 룸

	// 진짜 룸 만들기
	roomData_[SX][SY] = new Room(Room::RoomType::START);
	roomData_[EX][EY] = new Room(Room::RoomType::END);
}
// 시작 위치와 끝 위치를 이어주는 함수
// SX, SY를 변경하니 만약 원본이 필요하다면 따로 할당해놔야 한다.
void Floor_1::ConnectSRER(int& SX, int& SY, int& EX, int& EY)
{

	bool possibilityX = true;
	bool possibilityY = true;
	while (!(SX == EX) || !(SY == EY))
	{
		possibilityX = true;
		possibilityY = true;	// 시작 위치 기준 끝 위치가 상하좌우 어느 속성을 가지고 있는지 검사 ( d = distance )
		int dX = EX - SX;	// 음수면 left, 양수면 right
		int dY = SY - EY;    // 양수면 UP, 음수면 Down
		float diceX = 0.0f;
		float diceY = 0.0f;
		if ( dX == 0 )	// X는 확장되지 않음.
		{
			possibilityX = false;
		}
		if( dY == 0 ) // Y는 확장되지 않음.
		{
			possibilityY = false;
		}
		// 둘다 참일때만 이 복잡한 확률연산을
		if (possibilityX == true || possibilityY == true) {
			// 통과 못하면 0 이 되겠지?
			// 정수는 적용이 안되네?
		//	std::random_device rd;
		//	std::default_random_engine eng(rd());		
		//	std::uniform_real_distribution<int> dist(0, (abs(dX)) + (abs(dY)) - 1);

			int randn = rand() % ((abs(dX)) + (abs(dY)));
			printf("거리 절댓값:%d\n", (abs(dX)) + (abs(dY)));
			printf("출력된 random 값:%d\n", randn);
			if (randn - (abs(dY)) < 0) {
				// DY 당첨
				// Y를 올리거나 내려
				if (dY < 0)
					++SY;
				else
					--SY;
			}
			else {
				// DX 당첨
				// X를 올리거나 내려
				if (dX > 0)
					++SX;
				else
					--SX;
			}
			// 최종 반영 직전 검사 ( 위치가 end면 안됨 )
			if (tempRoom[SX][SY] != (int)Room::RoomType::END) {
				printf("\n시작위치: %d %d\n", SX, SY);
				// 동일 위치가 아니니 해당 위치를 NORMAL로 설정
				tempRoom[SX][SY] = Room::RoomType::NORMAL;
				// 진짜 룸 만들기
				roomData_[SX][SY] = new Room(Room::RoomType::NORMAL);
			}			
			else // 다 연결된거임. 시작과 끝이 같으니까				
				break;
			
		} // end if
	}//end while
}

// 방 연결하기 DFS 비슷한 탐색 알고리즘 ( 
void Floor_1::ConnectRoom(int x, int y)
{
	// 이미 방문했거나 End이면 경우 return
	if (visitedRoom[x][y] == true)
		return;
	else
		visitedRoom[x][y] = true;

	if (tempRoom[x][y] == (int)Room::RoomType::END)
		return;

	// 여기를 시작으로 Normal DFS 수행
	if (tempRoom[x][y] != (int)Room::RoomType::DISABLE) {	// disable이 아니면

		if (x+1 < maxX && tempRoom[x + 1][y] != (int)Room::RoomType::DISABLE) {
			roomData_[x][y]->myLinkedRoom_ = (Room::LinkedRoom)(roomData_[x][y]->myLinkedRoom_ | Room::LinkedRoom::RIGHT);
			ConnectRoom(x + 1,y);
		}
		else if (x - 1 >= 0 && tempRoom[x - 1][y] != (int)Room::RoomType::DISABLE) {
			roomData_[x][y]->myLinkedRoom_ = (Room::LinkedRoom)(roomData_[x][y]->myLinkedRoom_ | Room::LinkedRoom::LEFT);
			ConnectRoom(x - 1, y);
		}
		else if (y + 1 < maxY && tempRoom[x][y + 1] != (int)Room::RoomType::DISABLE) {
			roomData_[x][y]->myLinkedRoom_ = (Room::LinkedRoom)(roomData_[x][y]->myLinkedRoom_ | Room::LinkedRoom::BOTTOM);
			ConnectRoom(x, y + 1);
		}
		else if (y - 1 >= 0 && tempRoom[x][y - 1] != (int)Room::RoomType::DISABLE) {
			roomData_[x][y]->myLinkedRoom_ = (Room::LinkedRoom)(roomData_[x][y]->myLinkedRoom_ | Room::LinkedRoom::TOP);
			ConnectRoom(x, y - 1);
		}
	}		
}
// 미완성: 위에처럼 탐색해가며 맵의 데이터를 바탕으로 이미지를 정해줘야함.
void Floor_1::GenerateRoomData()
{
	// 일단 new 자체는 이전에 되어있음. 빈 깡통이라 그렇지
	int x = currentActiveRoom_[0];
	int y = currentActiveRoom_[1];
	roomData_[x][y]->InitializeRoom();
	roomData_[x][y]->myIndex[0] = x;
	roomData_[x][y]->myIndex[1] = y;
}
