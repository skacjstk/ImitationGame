#include "ImitationGame/framework.h"
#include "Physics/Collider.h"
#include "State/State.h"
#include "State/SkelDogState.h"
#include "State/IdleState.h"
#include "State/RunState.h"
#include "State/AttackState.h"
#include "SkelDog.h"

SkelDog::SkelDog()
{
	SetActive(false);
	state_[0] = new IdleState();
	state_[1] = new RunState();
	state_[2] = new AttackState();
	currentState_ = state_[static_cast<int>(stateEnum_)];	

	wstring strImage = IMAGE_FOLDER; strImage += L"Monster/SkelDog/SkelDogDie.png";
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";

	_animation = new Animation(strImage, strShader);

	// IDLE
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);

		for (int i = 0; i <= 4; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Monster/SkelDog/SkelDogIdle" + to_wstring(i) + L".png";
			pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);

		}
		_animation->AddClip(pClip);
	}
	// RUN
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);

		for (int i = 0; i <= 6; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Monster/SkelDog/SkelDogRun" + to_wstring(i) + L".png";
			pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);

		}
		_animation->AddClip(pClip);
	}
	// Attack	// Run 0번 재탕
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		strImage = IMAGE_FOLDER; strImage += L"Monster/SkelDog/SkelDogRun0.png";
		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);		
		_animation->AddClip(pClip);
	}
	// 배율 설정 및 충돌체 할당
	pCollider_ = new Collider();
	_animation->SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
}

SkelDog::~SkelDog()
{
	SAFE_DELETE(_animation);
	SAFE_DELETE(currentState_);
	for(int i=0;i<_countof(state_);++i)
		SAFE_DELETE(state_[i]);
	SAFE_DELETE(pCollider_);
}

void SkelDog::Update(Matrix V, Matrix P)
{
	if (IsActive() == false)
		return;
	// IDLE, RUN, ATTACK(RUN 0 번 재탕으로 뛰어오르기)
	GroundCheck();
	StateUpdate();
	_animation->SetPlay(static_cast<int>(stateEnum_));
	_animation->SetPosition(GetPosition());
	_animation->SetRotation(GetRotation());
	_animation->Update(V, P);

	pCollider_->SetPosition(GetPosition());
	pCollider_->SetRotation(GetRotation());
	pCollider_->Update(V, P);
}

void SkelDog::Render()
{
	if (IsActive() == false)
		return;
	_animation->Render();
	pCollider_->Render();
}

void SkelDog::Reset()
{
	SetActive(true);
	SetScale(3.0f * WSCALEX, 3.0f * WSCALEY);
	_animation->SetScale(GetScale());
	pCollider_->SetScale(_animation->GetTextureRealSize());
	chaseTarget_ = (GameActor*)OBJECTMANAGER->FindObject("player");
	this->SetPosition(chaseTarget_->GetPosition());
}

void SkelDog::StateUpdate()
{
	currentState_->SwitchState(*this);
	currentState_->Action(*this);
}

void SkelDog::CollisionCheck()
{
}

void SkelDog::AttackCheck()
{
}

void SkelDog::LeftMove()
{
}

void SkelDog::RightMove()
{
}

void SkelDog::Jump()
{
}

void SkelDog::Move()
{
	printf("해골개 Move\n");
}

void SkelDog::Idle()
{
	printf("해골개 Idle\n");
}

void SkelDog::Attack()
{
}
