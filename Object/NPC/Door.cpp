#include "ImitationGame/framework.h"
#include "Object/Player.h"
#include "Object/NPC.h"
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
	doorState = DoorState::CLOSING;
	ppPlayer = (Player*)OBJECTMANAGER->FindObject("player");
	this->SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
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
// ���� ���� ���¿����� �÷��̾� ��ġ�˻� ��
void Door::OpenSwitch()
{
	// Ŀ�´����̼� ��ư Ȱ��ȭ ������ Communicate() ����
	if (CheckPlayer() == true) {
		printf("�����\n");
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
	doorState = DoorState::CLOSING;
	_animation->SetPlay(doorState);
}
// ��� �� ������ �ҰԾ���. �׳� Texture ��
void Door::CloseSwitch()
{
}

void Door::CloseEnter()
{
	doorState = DoorState::CLOSE;
	_animation->SetPlay(doorState);
}
