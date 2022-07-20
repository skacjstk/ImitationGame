#include "ImitationGame/framework.h"
#include "Physics/Collider.h"
#include "State/State.h"
#include "State/SkelDogState.h"
#include "State/IdleState.h"
#include "State/RunState.h"
#include "State/AttackState.h"
#include "State/JumpState.h"
#include "State/WaitState.h"
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
	// Attack	// Run 0�� ����
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		strImage = IMAGE_FOLDER; strImage += L"Monster/SkelDog/SkelDogRun0.png";
		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);		
		_animation->AddClip(pClip);
	}
	// JUMP	// Run 0�� ����
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		strImage = IMAGE_FOLDER; strImage += L"Monster/SkelDog/SkelDogRun0.png";
		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);
		_animation->AddClip(pClip);
	}
	// Wait	// IDLE 0�� ����
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		strImage = IMAGE_FOLDER; strImage += L"Monster/SkelDog/SkelDogIdle0.png";
		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);
		_animation->AddClip(pClip);
	}
	// ���� ���� �� �浹ü �Ҵ�
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
	// IDLE, RUN, ATTACK(RUN 0 �� �������� �پ������)
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
	// ������ ������ ���� ����, Ÿ���� �����ð��� 0 �̸鼭
	if (chaseTarget_->GetImmuneFrame() <= 0 && Collider::IntersectAABB(pCollider_, chaseTarget_->GetCollider()))
	{
		chaseTarget_->Attacked();
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
	float sign = copysign(1, dX);	// copysign ��ȣ ��Ʈ ���� ��¶�� ����� 1, ������ -1
	ModifyPosition(sign * moveSpeed * TIMEMANAGER->Delta(), 0.0f);	// Next: -1 ���ؾ� �� �� ����.

	sign -= 1.0f;	// ��� 0, ���� -2
	SetRotation(0.0f, sign * 90.0f, 0.0f);

	if (isGround_ == true) {
		isJump = true;
		isGround_ = false;
		gravity_ = jumpSpeed * 0.1f;
	}
}

void SkelDog::Move()
{
	// chaseTarget_ ����
	float dX = chaseTarget_->GetPosition().x - GetPosition().x;
	float sign = copysign(1, dX);	// copysign ��ȣ ��Ʈ ���� ��¶�� ����� 1, ������ -1
	ModifyPosition(sign * moveSpeed * TIMEMANAGER->Delta(), 0.0f);	// Next: -1 ���ؾ� �� �� ����.

	sign -= 1.0f;	// ��� 0, ���� -2
	SetRotation(0.0f, sign * 90.0f, 0.0f);
}

void SkelDog::Idle()
{
	// �ϴ°� ��� �Ҵ� ����.
}
// �ִϸ��̼� ��ü�� Move�� Jump�� ����.
void SkelDog::Attack()
{
	printf("skelDog�� dX�� �޴� ���� Attack�� �����");
}

// ������ ������ dX�� �� �ѹ��� ������.
void SkelDog::Attack(float& dX)
{
	Vector2 tempPosition = GetPosition();
	float sign = copysign(1, dX);	// copysign ��ȣ ��Ʈ ���� ��¶�� ����� 1, ������ -1
	ModifyPosition(sign * moveSpeed * TIMEMANAGER->Delta(), 0.0f);	// Next: -1 ���ؾ� �� �� ����.

	sign -= 1.0f;	// ��� 0, ���� -2
	SetRotation(0.0f, sign * 90.0f, 0.0f);
	if (isGround_ == true) {
		isJump = true;
		isGround_ = false;
		gravity_ = jumpSpeed * 0.05f;
	}
	AttackCheck();
}
