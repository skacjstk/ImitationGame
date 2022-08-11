#include "ImitationGame/framework.h"
#include "Object/Player.h"
#include "Object/NPC.h"
#include "Physics/Collider.h"
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
		pClip->AddFrame(pTexture, strImage, 0, 0, 57, 80, 1.0f);
		
		_animation->AddClip(pClip);
	}
	// CLOSING
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::End);
		Texture* pTexture = _animation->GetTexture();
		for (int i = 1; i <= 10; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"NPC/Door/Door (" + to_wstring(i) +  L").png";
			pClip->AddFrame(pTexture, strImage, 0, 0, 57, 80, 0.1f);
		}
		_animation->AddClip(pClip);
	}
	// CLOSE
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		Texture* pTexture = _animation->GetTexture();
		strImage = IMAGE_FOLDER; strImage += L"NPC/Door/Door (10).png";
		pClip->AddFrame(pTexture, strImage, 0, 0, 57, 80, 1.0f);

		_animation->AddClip(pClip);
	}

	// NPC 코드 설정
	codeNPC_ = 101;	// DB에서 찾을때만 써.
	actorData_.type = ActorType::Other;
	pCollider_ = new Collider();
}

Door::~Door()
{
}

void Door::Update(Matrix V, Matrix P)
{
	if (IsActive() == false)
		return;

	SwitchState();

	_animation->SetScale(GetScale());
	_animation->SetPosition(GetPosition());
	_animation->Update(V, P);
	pCollider_->Update(V, P);
}

void Door::Render()
{
	if (IsActive() == false)
		return;
	_animation->Render();
	pCollider_->Render();
}

void Door::Reset()
{
	SetActive(true);
	doorState = DoorState::CLOSING;
	ppPlayer = (Player*)OBJECTMANAGER->FindObject("player");
	this->SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
	_animation->SetScale(this->GetScale());

	pCollider_->SetScale(_animation->GetTextureRealSize());
	pCollider_->SetPosition(GetPosition());


	// Reset이 맵 진입 시점일 때 라고 가정.
	// 이 시점에서, 만약 자기가 위치한 Room 의 값이 END 일 경우 OPEN으로 변경
	// 아닐 경우 Closing으로 변경
	if (SCENEMANAGER->GetCurrentScene()->GetCurrentRoom()->roomType_ == Room::RoomType::END) {
		SwitchState = std::bind(&Door::OpenSwitch, this);
		Enter = std::bind(&Door::OpenSwitch, this);
	}
	else {
		SwitchState = std::bind(&Door::ClosingSwitch, this);
		Enter = std::bind(&Door::ClosingEnter, this);
	}
//	SwitchState = std::bind(&Door::ClosingSwitch, this);
//	Enter = std::bind(&Door::ClosingEnter, this);
	Enter();

}

void Door::Communicate()
{
	printf("Door CloseSwitch에 테스트코드 있음\n");
	// 충돌 검사를 또 하는 이유: 한번 닿아놓고 멀리 간다음에 F 누르면 안되니까.
	if(CheckPlayer())
		eventHandler->Push(L"RiseNextFloor");

}

bool Door::CheckPlayer()
{
	return Collider::IntersectAABB(pCollider_, ppPlayer->GetCollider());
}
// 문이 열린 상태에서는 플레이어 위치검사 및
void Door::OpenSwitch()
{
	// 커뮤니케이션 버튼 활성화 누르면 Communicate() 진입
	if (CheckPlayer() == true) {
		// 자신을 상호작용 대상으로 등록
		ppPlayer->SetInteractionTarget(this);
	}
}

void Door::OpenEnter()
{
	doorState = DoorState::OPEN;
	_animation->SetPlay(doorState);
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
	Audio->Play("JailClose");
	doorState = DoorState::CLOSING;
	_animation->SetPlay(doorState);
}
// 얘는 뭐 닫히면 할게없어. 그냥 Texture 야
void Door::CloseSwitch()
{
	// 테스트코드: Open 으로 바뀜: 다음 층 바로 가려고 Next 삭제
//	SwitchState = std::bind(&Door::OpenSwitch, this);
//	Enter = std::bind(&Door::OpenEnter, this);
//	Enter();
}

void Door::CloseEnter()
{
	doorState = DoorState::CLOSE;
	_animation->SetPlay(doorState);
}
