#include "ImitationGame/framework.h"
#include "Physics/Collider.h"
#include "Object/Player.h"
#include "Object/NPC.h"
#include "Stele.h"

Stele::Stele()
{
	SetActive(false);
	wstring shader = SHADER_FOLDER;	shader += L"TextureColor.hlsl";
	wstring strImage = IMAGE_FOLDER;	strImage += L"NPC/Stele/Stele00.png";	// ���� ū �κ� �̹���
	_animation = new Animation(strImage, shader);


	// OPEN	(���ڰ� �귯���;� ������...)
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		Texture* pTexture = _animation->GetTexture();
		strImage = IMAGE_FOLDER; strImage += L"NPC/Stele/Stele00.png";
		pClip->AddFrame(pTexture, strImage, 0, 0, 66, 20, 1.0f);
		
		_animation->AddClip(pClip);
	}
	// OPENING
	{	
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::End);
		Texture* pTexture = _animation->GetTexture();
		for (int i = 16; i <= 22; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"NPC/Stele/Stele" 
				+ to_wstring((int)(i * 0.1)) + to_wstring(i % 10) + L".png";
			pClip->AddFrame(pTexture, strImage, 0, 0, 66, 20, 0.08f);
		}
		_animation->AddClip(pClip);
	}
	// CLOSING
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::End);
		Texture* pTexture = _animation->GetTexture();
		for (int i = 0; i <= 8; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"NPC/Stele/Stele"
				+ to_wstring((int)(i * 0.1)) + to_wstring(i % 10) + L".png";
			pClip->AddFrame(pTexture, strImage, 0, 0, 66, 20, 0.08f);
		}
		_animation->AddClip(pClip);
	}
	// CLOSE
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		Texture* pTexture = _animation->GetTexture();
		for (int i = 8; i <= 15; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"NPC/Stele/Stele"
				+ to_wstring((int)(i * 0.1)) + to_wstring(i % 10) + L".png";
			pClip->AddFrame(pTexture, strImage, 0, 0, 66, 20, 0.075f);
		}
		_animation->AddClip(pClip);
	}

	// ��ƼŬ �߰�
	int particleList[4] = { 0,1,3,5 };

	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distr(0, 3);

	for (UINT i = 0; i < particles_.size(); ++i) {
		strImage = IMAGE_FOLDER; strImage += L"Particle/Particle" + to_wstring(particleList[distr(eng)]) + L".png";
		particles_[i] = new Texture(strImage, shader);
	}

	pCollider_ = new Collider();
	actorData_.type = ActorType::Other;
}

Stele::~Stele()
{
}

void Stele::Update(Matrix V, Matrix P)
{
	if (IsActive() == false)
		return;

	SwitchState();

	_animation->SetScale(GetScale());
	_animation->SetPosition(GetPosition());
	_animation->SetRotation(GetRotation());
	_animation->Update(V, P);

	pCollider_->SetPosition(GetPosition());
	pCollider_->SetPosition(GetPosition());
	pCollider_->SetRotation(GetRotation());
	pCollider_->Update(V, P);
	/*
	�޸�:
	�浹 ���� ��ü�� line�� collider�� ���ϴ� (player�� intersectionLine)
	�װɷ� �ϰ�, Open ���¿��� �ε����� �Ѱ��ֱ�, �ƴϸ� ����� ���������� ����
	*/
}

void Stele::Render()
{
	if (IsActive() == false)
		return;
	_animation->Render();
	pCollider_->Render();
}

void Stele::Reset()
{
	SetActive(true);
	SteleState_ = SteleState::CLOSING;
	ppPlayer = (Player*)OBJECTMANAGER->FindObject("player");
	this->SetScale(TRNMANAGER->GetMapScale().x, TRNMANAGER->GetMapScale().y);
	_animation->SetScale(this->GetScale());
	
	pCollider_->SetScale(_animation->GetTextureRealSize());

	SetPath();

	SwitchState = std::bind(&Stele::ClosingSwitch, this);
	Action = std::bind(&Stele::ClosingAction, this);
	Enter = std::bind(&Stele::ClosingEnter, this);
	Enter();

}

void Stele::SetPath()
{
	float angle = GetRotation().z;

	if (fabsf(angle - 0.0f) < FLT_EPSILON)	{
		stelePath_ = StelePath::BOTTOM;		printf("�� ��Ż\n");
	}
	else if (fabsf(angle - 90.0f) < FLT_EPSILON)	{
		stelePath_ = StelePath::RIGHT;		printf("�� ��Ż\n");
	}
	else if (fabsf(angle - 180.0f) < FLT_EPSILON)	{
		stelePath_ = StelePath::TOP;		printf("�� ��Ż\n");
	}
	else if (fabsf(angle - 270.0f) < FLT_EPSILON)	{
		stelePath_ = StelePath::LEFT;		printf("�� ��Ż\n");
	}
}
bool Stele::CheckPlayer()
{
	return Collider::Obb(ppPlayer->GetCollider(), pCollider_);	// stele�� ȸ���� �ֱ⿡ Obb�� �˻��ؾ� ��.

}
// ���� ���� ���¿����� �÷��̾� ��ġ�˻� ��
void Stele::OpenSwitch()
{
	
	if (CheckPlayer() == true) {
		// �̵� �̺�Ʈ ȣ��.
		switch (stelePath_)
		{
		case StelePath::BOTTOM:
			eventHandler->Push(L"MR1");	// ��
			break;
		case StelePath::LEFT:
			eventHandler->Push(L"MR2");	// ��
			break;
		case StelePath::TOP:
			eventHandler->Push(L"MR3");	// ��
			break;
		case StelePath::RIGHT:
			eventHandler->Push(L"MR4");	// ��
			break;
		}

		SwitchState = std::bind(&Stele::ClosingSwitch, this);
		Action = std::bind(&Stele::ClosingEnter, this);
		Enter = std::bind(&Stele::ClosingEnter, this);
		Enter();
	}

}

void Stele::OpenAction()
{
	// ���⼱ ��ƼŬ�� �վ��� ���;� ��.
}

void Stele::OpenEnter()
{
	SteleState_ = SteleState::OPEN;
	_animation->SetPlay(SteleState_);
}
// �ִϸ��̼��� ������, ������ Open ���� �ٲ�.
void Stele::OpeningSwitch()
{
	if (_animation->IsPlay() == false) {
		SwitchState = std::bind(&Stele::OpenSwitch, this);
		Action = std::bind(&Stele::OpenEnter, this);
		Enter = std::bind(&Stele::OpenEnter, this);
		Enter();
	}
}

void Stele::OpeningAction()
{
}

void Stele::OpeningEnter()
{
	SteleState_ = SteleState::OPENING;
	_animation->SetPlay(SteleState_);
}



// Closing �� ������ Close�� �ٲ�.
void Stele::ClosingSwitch()
{
	if (_animation->IsPlay() == false)
	{
		SwitchState = std::bind(&Stele::CloseSwitch, this);
		Action = std::bind(&Stele::CloseEnter, this);
		Enter = std::bind(&Stele::CloseEnter, this);
		Enter();
	}
}

void Stele::ClosingAction()
{
}

void Stele::ClosingEnter()
{
	SteleState_ = SteleState::CLOSING;
	_animation->SetPlay(SteleState_);
}

void Stele::CloseSwitch()
{
	// ���� Ŭ���� �̺�Ʈ�� �߻��� ���, Next
	SwitchState = std::bind(&Stele::OpeningSwitch, this);
	Action = std::bind(&Stele::OpeningEnter, this);
	Enter = std::bind(&Stele::OpeningEnter, this);
	Enter();
}
void Stele::CloseAction()
{
}

void Stele::CloseEnter()
{
	SteleState_ = SteleState::CLOSE;
	_animation->SetPlay(SteleState_);
}
