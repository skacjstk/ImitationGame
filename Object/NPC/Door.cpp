#include "ImitationGame/framework.h"
#include "Object/Player.h"
#include "Object/NPC.h"
#include "Physics/Collider.h"
#include "Door.h"

Door::Door()
{
	SetActive(false);
	wstring shader = SHADER_FOLDER;	shader += L"TextureColor.hlsl";
	wstring strImage = IMAGE_FOLDER;	strImage += L"NPC/Door/Door (10).png";	// ���� ū �κ� �̹���
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

	// NPC �ڵ� ����
	codeNPC_ = 101;	// DB���� ã������ ��.
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
	if (isRise_ == true) {
		fadeTimeCheck_ += TIMEMANAGER->Delta();
		if (fadeTimeCheck_ >= 1.0f) {
			RiseNextFloor();
		}
	}

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


	// Reset�� �� ���� ������ �� ��� ����.
	// �� ��������, ���� �ڱⰡ ��ġ�� Room �� ���� END �� ��� OPEN���� ����
	// �ƴ� ��� Closing���� ����
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
	printf("Door CloseSwitch�� �׽�Ʈ�ڵ� ����\n");
	// �浹 �˻縦 �� �ϴ� ����: �ѹ� ��Ƴ��� �ָ� �������� F ������ �ȵǴϱ�.
	if (CheckPlayer()) {
		CAMERA->FadeOut(1.0f);
		isRise_ = true;	// FadeOut�� �������� �ð��� ������ fade �̺�Ʈ�� ȣ���� ����
	}

}

bool Door::CheckPlayer()
{
	return Collider::IntersectAABB(pCollider_, ppPlayer->GetCollider());
}
void Door::RiseNextFloor()
{
	eventHandler->Push(L"RiseNextFloor");
}
// ���� ���� ���¿����� �÷��̾� ��ġ�˻� ��
void Door::OpenSwitch()
{
	// Ŀ�´����̼� ��ư Ȱ��ȭ ������ Communicate() ����
	if (CheckPlayer() == true) {
		// �ڽ��� ��ȣ�ۿ� ������� ���
		ppPlayer->SetInteractionTarget(this);
	}
}

void Door::OpenEnter()
{
	doorState = DoorState::OPEN;
	_animation->SetPlay(doorState);
}
// Closing �� ������ Close�� �ٲ�.
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
// ��� �� ������ �ҰԾ���. �׳� Texture ��
void Door::CloseSwitch()
{
	// �׽�Ʈ�ڵ�: Open ���� �ٲ�: ���� �� �ٷ� ������ Next ����
//	SwitchState = std::bind(&Door::OpenSwitch, this);
//	Enter = std::bind(&Door::OpenEnter, this);
//	Enter();
}

void Door::CloseEnter()
{
	doorState = DoorState::CLOSE;
	_animation->SetPlay(doorState);
}
