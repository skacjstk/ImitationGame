#include "ImitationGame/framework.h"
#include "Object/Player.h"
#include "Object/NPC.h"
#include "Door.h"

Door::Door()
{
	SetActive(false);
	wstring shader = SHADER_FOLDER;	shader += L"TextureColor.hlsl";
	wstring strImage = IMAGE_FOLDER;	strImage += L"NPC/Door/Door (10).png";	// 제일 큰 부분 이미지
	_animation = new Animation(strImage, shader);


	// OPEN
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		Texture* pTexture = _animation->GetTexture();
		strImage = IMAGE_FOLDER; strImage += L"NPC/Door/Door (1).png";
		pClip->AddFrame(pTexture, strImage, 0, 0, 57, 65, 1.0f);
		
		_animation->AddClip(pClip);
	}
	// CLOSING
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::End);
		Texture* pTexture = _animation->GetTexture();
		for (int i = 1; i <= 10; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"NPC/Door/Door (" + to_wstring(i) +  L").png";
			pClip->AddFrame(pTexture, strImage, 0, 0, 57, 65, 1.0f);
		}
		_animation->AddClip(pClip);
	}
	// CLOSE
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		Texture* pTexture = _animation->GetTexture();
		strImage = IMAGE_FOLDER; strImage += L"NPC/Door/Door (10).png";
		pClip->AddFrame(pTexture, strImage, 0, 0, 57, 65, 1.0f);

		_animation->AddClip(pClip);
	}
}

Door::~Door()
{
}

void Door::Update(Matrix V, Matrix P)
{
	if (IsActive() == false)
		return;

	SwitchState();
	Action();

	_animation->SetScale(GetScale());
	_animation->SetPosition(GetPosition());
	_animation->Update(V, P);
}

void Door::Render()
{
	if (IsActive() == false)
		return;
	_animation->Render();
}

void Door::Reset()
{
	SetActive(true);
	_currentState = DoorState::CLOSING;
	ppPlayer = (Player*)OBJECTMANAGER->FindObject("player");
	this->SetScale(1.0f * WSCALEX, 1.0f * WSCALEY);	// 일단 이건 절대 6배 아니야
	_animation->SetScale(this->GetScale());

	SwitchState = std::bind(&Door::ClosingSwitch, this);
	Enter = std::bind(&Door::ClosingEnter, this);

	Enter();
}

void Door::Communicate()
{
	eventHandler->Push(L"RiseNextFloor");
//	SCENEMANAGER->ChangeScene("Floor_" + to_wstring(currentFloor_ + 1));
}

bool Door::CheckPlayer()
{
	Vector2 tempPosition = (*ppPlayer).GetPosition();
	float x = this->_position.x - tempPosition.x;
	float y = this->_position.y - tempPosition.y;

	if (fabsf(x) < 10.0f * WSCALEX && fabsf(y) < 30.0f * WSCALEY)
	{
		return true;
	}
	return false;
}
// 문이 열린 상태에서는 플레이어 위치검사 및
void Door::OpenSwitch()
{
	// 커뮤니케이션 버튼 활성화 누르면 Communicate() 진입
	if (CheckPlayer() == true) {
		printf("닿았음\n");
	}
}

void Door::OpenEnter()
{
	_currentState = DoorState::OPEN;
	_animation->SetPlay(_currentState);
}
// Closing 이 끝나면 Close로 바뀜.
void Door::ClosingSwitch()
{
	if (_animation->IsPlay() == false)
	{
		SwitchState = std::bind(&Door::CloseSwitch, this);
		Enter = std::bind(&Door::CloseEnter, this);
		Enter();
	}
}

void Door::ClosingEnter()
{
	_currentState = DoorState::CLOSING;
	_animation->SetPlay(_currentState);
}
// 얘는 뭐 닫히면 할게없어. 그냥 Texture 야
void Door::CloseSwitch()
{
}

void Door::CloseEnter()
{
	_currentState = DoorState::CLOSE;
	_animation->SetPlay(_currentState);
}
