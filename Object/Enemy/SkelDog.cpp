#include "ImitationGame/framework.h"
#include "Physics/Collider.h"
#include "State/State.h"
#include "State/SkelDogState/SkelDogState.h"
#include "State/SkelDogState/IdleState.h"
#include "State/SkelDogState/RunState.h"
#include "State/SkelDogState/AttackState.h"
#include "State/SkelDogState/JumpState.h"
#include "State/SkelDogState/WaitState.h"
#include "SkelDog.h"

SkelDog::SkelDog()
{
	SetActive(false);
	state_[0] = new IdleState();
	state_[1] = new RunState();
	state_[2] = new AttackState();
	state_[3] = new JumpState();
	state_[4] = new WaitState();
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
	// JUMP	// Run 0번 재탕
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		strImage = IMAGE_FOLDER; strImage += L"Monster/SkelDog/SkelDogRun0.png";
		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);
		_animation->AddClip(pClip);
	}
	// Wait	// IDLE 0번 재탕
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		strImage = IMAGE_FOLDER; strImage += L"Monster/SkelDog/SkelDogIdle0.png";
		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);
		_animation->AddClip(pClip);
	}
	// 배율 설정 및 충돌체 할당
	pCollider_ = new Collider();
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
	StateUpdate();
	GroundCheck();
	GravityUpdate();
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
	SetScale(3.0f * WSCALEX, 3.0f * WSCALEY);
	_animation->SetScale(GetScale());
	pCollider_->SetScale(_animation->GetTextureRealSize());
	chaseTarget_ = (GameActor*)OBJECTMANAGER->FindObject("player");
	this->SetPosition(chaseTarget_->GetPosition());
	actorData_.HP = 10;
	actorData_.maxHP = 10;
	actorData_.ImmuneTime = 0;
	SetActive(true);
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
	// 겹쳐져 있으면 공격 성공, 타겟의 무적시간이 0 이면서
	if (chaseTarget_->GetImmuneFrame() <= 0 && Collider::IntersectAABB(pCollider_, chaseTarget_->GetCollider()))
	{
		chaseTarget_->Attacked(actorData_.damage);
	}
}

void SkelDog::LeftMove()
{
}

void SkelDog::RightMove()
{
}

void SkelDog::Jump()
{
	Vector2 tempPosition = GetPosition();
	float dX = chaseTarget_->GetPosition().x - GetPosition().x;
	float sign = copysign(1, dX);	// copysign 부호 비트 추출 어쨋든 양수면 1, 음수면 -1
	ModifyPosition(sign * moveSpeed * TIMEMANAGER->Delta(), 0.0f);	// Next: -1 곱해야 할 수 있음.

	sign -= 1.0f;	// 양수 0, 음수 -2
	SetRotation(0.0f, sign * 90.0f, 0.0f);

	if (isGround_ == true) {
		isJump = true;
		isGround_ = false;
		gravity_ = jumpSpeed * 0.07f;
	}
}

void SkelDog::Move()
{
	// chaseTarget_ 추적
	float dX = chaseTarget_->GetPosition().x - GetPosition().x;
	float sign = copysign(1, dX);	// copysign 부호 비트 추출 어쨋든 양수면 1, 음수면 -1
	ModifyPosition(sign * moveSpeed * TIMEMANAGER->Delta(), 0.0f);	// Next: -1 곱해야 할 수 있음.

	sign -= 1.0f;	// 양수 0, 음수 -2
	SetRotation(0.0f, sign * 90.0f, 0.0f);
}

void SkelDog::Idle()
{
	// 하는거 없어서 할당 안함.
}
// 애니메이션 자체는 Move와 Jump의 조합.
void SkelDog::Attack()
{
	printf("skelDog는 dX를 받는 개별 Attack을 사용함");
}

// 점프와 같지만 dX는 딱 한번만 연산함.
void SkelDog::Attack(float& dX)
{
	Vector2 tempPosition = GetPosition();
	float sign = copysign(1, dX);	// copysign 부호 비트 추출 어쨋든 양수면 1, 음수면 -1
	ModifyPosition(sign * moveSpeed * TIMEMANAGER->Delta(), 0.0f);	// Next: -1 곱해야 할 수 있음.

	sign -= 1.0f;	// 양수 0, 음수 -2
	SetRotation(0.0f, sign * 90.0f, 0.0f);
	AttackJump();
	AttackCheck();
}
// 한번만 점프함
void SkelDog::AttackJump()
{
	if (isGround_ == true && attacked_ == false) {
		attacked_ = true;
		isJump = true;
		isGround_ = false;
		gravity_ = jumpSpeed * 0.05f;
	}
}
