#include "ImitationGame/framework.h"
#include "Physics/Collider.h"
#include "Object/Player.h"
#include "Object/NPC.h"
#include "Stele.h"

Stele::Stele()
{
	SetActive(false);
	wstring shader = SHADER_FOLDER;	shader += L"TextureColor.hlsl";
	wstring strImage = IMAGE_FOLDER;	strImage += L"NPC/Stele/Stele00.png";	// 제일 큰 부분 이미지
	_animation = new Animation(strImage, shader);


	// OPEN	(입자가 흘러나와야 하지만...)
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

	// 파티클 추가
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
	메모:
	충돌 판정 자체를 line과 collider를 비교하는 (player에 intersectionLine)
	그걸로 하고, Open 상태에서 부딧히면 넘겨주기, 아니면 평범한 벽판정으로 하자
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
		stelePath_ = StelePath::BOTTOM;		printf("하 포탈\n");
	}
	else if (fabsf(angle - 90.0f) < FLT_EPSILON)	{
		stelePath_ = StelePath::RIGHT;		printf("우 포탈\n");
	}
	else if (fabsf(angle - 180.0f) < FLT_EPSILON)	{
		stelePath_ = StelePath::TOP;		printf("상 포탈\n");
	}
	else if (fabsf(angle - 270.0f) < FLT_EPSILON)	{
		stelePath_ = StelePath::LEFT;		printf("좌 포탈\n");
	}
}
bool Stele::CheckPlayer()
{
	return Collider::Obb(ppPlayer->GetCollider(), pCollider_);	// stele는 회전이 있기에 Obb로 검사해야 함.

}
// 문이 열린 상태에서는 플레이어 위치검사 및
void Stele::OpenSwitch()
{
	
	if (CheckPlayer() == true) {
		// 이동 이벤트 호출.
		switch (stelePath_)
		{
		case StelePath::BOTTOM:
			eventHandler->Push(L"MR1");	// 하
			break;
		case StelePath::LEFT:
			eventHandler->Push(L"MR2");	// 좌
			break;
		case StelePath::TOP:
			eventHandler->Push(L"MR3");	// 상
			break;
		case StelePath::RIGHT:
			eventHandler->Push(L"MR4");	// 우
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
	// 여기선 파티클이 뿜어져 나와야 함.
}

void Stele::OpenEnter()
{
	SteleState_ = SteleState::OPEN;
	_animation->SetPlay(SteleState_);
}
// 애니메이션이 끝나면, 완전히 Open 으로 바뀜.
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



// Closing 이 끝나면 Close로 바뀜.
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
	// 던전 클리어 이벤트가 발생할 경우, Next
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
