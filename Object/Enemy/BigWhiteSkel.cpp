#include "ImitationGame/framework.h"
#include "Physics/Collider.h"
#include "State/State.h"
#include "State/BigWhiteSkelState/BigWhiteSkelState.h"
#include "State/BigWhiteSkelState/IdleStateBWS.h"
#include "State/BigWhiteSkelState/MoveStateBWS.h"
#include "State/BigWhiteSkelState/AttackStateBWS.h"
#include "State/BigWhiteSkelState/JumpStateBWS.h"
#include "State/BigWhiteSkelState/WaitStateBWS.h"
#include "BigWhiteSkel.h"

BigWhiteSkel::BigWhiteSkel()
{
	SetActive(false);
	state_[0] = new IdleStateBWS();
	state_[1] = new RunStateBWS();
	state_[2] = new AttackStateBWS();
	state_[3] = new JumpStateBWS();
	state_[4] = new WaitStateBWS();
	currentState_ = state_[static_cast<int>(stateEnum_)];	

	wstring strImage = IMAGE_FOLDER; strImage += L"Monster/BigWhiteSkel/BigWhiteSkelIdle.png";
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";

	_animation = new Animation(strImage, strShader);

	// IDLE
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);

		for (int i = 0; i <= 4; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Monster/BigWhiteSkel/BigWhiteSkelIdle" + to_wstring(i) + L".png";
			pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);

		}
		_animation->AddClip(pClip);
	}
	// MOVE
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);

		for (int i = 0; i <= 5; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Monster/BigWhiteSkel/BigWhiteSkelMove" + to_wstring(i) + L".png";
			pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);

		}
		_animation->AddClip(pClip);
	}
	// Attack	// 00 ~ 12
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::End);
		float temp[13][4] = {
		// 자체배율이 3.0이라 1/2 인 1.5로 했음.
		{35.0f,31.0f, -2.0f	* 1.5f * WSCALEX			, 1.0f	* -1.5f * WSCALEY},
		{36.0f,31.0f, -3.0f * 1.5f * WSCALEX			, 1.0f	* -1.5f * WSCALEY},
		{61.0f,48.0f, -28.0f * 1.5f * WSCALEX			, 18.0f	* -1.5f * WSCALEY},
		{57.0f,48.0f, (- 14.0f -24.0f) * 1.5f * WSCALEX			, 18.0f	* -1.5f * WSCALEY},	//
		{58.0f,40.0f, (- 14.0f -25.0f) * 1.5f * WSCALEX			, 10.0f	* -1.5f * WSCALEY},
		{58.0f,26.0f, (- 14.0f -25.0f) * 1.5f * WSCALEX			, -4.0f	* -1.5f * WSCALEY},
		{45.0f,26.0f, (- 14.0f -12.0f) * 1.5f * WSCALEX			, -4.0f * -1.5f * WSCALEY},
		{51.0f,30.0f, (- 14.0f -18.0f) * 1.5f * WSCALEX	 		, 0.0f	* -1.5f * WSCALEY},
		{54.0f,48.0f, (- 14.0f -21.0f) * 1.5f * WSCALEX			, 18.0f	* -1.5f * WSCALEY},	//
		{65.0f,48.0f, -32.0f * 1.5f * WSCALEX			, 18.0f	* -1.5f * WSCALEY},
		{62.0f,48.0f, -29.0f * 1.5f * WSCALEX			, 18.0f	* -1.5f * WSCALEY},
		{36.0f,34.0f, -3.0f * 1.5f * WSCALEX			, 4.0f	* -1.5f * WSCALEY},
		{33.0f,30.0f, 0.0f, 0.0f}	
		// 중간 프레임에 14는 7 x 2 로써, 모션 진행중 해골 몸체 시작점이 바뀌기 때문		
		};
		for (int i = 0; i <= 12; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Monster/BigWhiteSkel/BigWhiteSkelAttack" + to_wstring((int)(i * 0.1)) + to_wstring(i % 10) + L".png";
			pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, temp[i][0], temp[i][1], 0.075f, temp[i][2], temp[i][3]);
	//		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);
		}
		_animation->AddClip(pClip);
	}
	// JUMP	// Idle 0번 재탕
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		strImage = IMAGE_FOLDER; strImage += L"Monster/BigWhiteSkel/BigWhiteSkelIdle.png";
		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);
		_animation->AddClip(pClip);
	}
	// Wait	// IDLE 0번 재탕
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		strImage = IMAGE_FOLDER; strImage += L"Monster/BigWhiteSkel/BigWhiteSkelIdle.png";
		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);
		_animation->AddClip(pClip);
	}
	// 배율 설정 및 충돌체 할당
	pCollider_ = new Collider();
}

BigWhiteSkel::~BigWhiteSkel()
{
	SAFE_DELETE(_animation);
	SAFE_DELETE(currentState_);
	for(int i=0;i<_countof(state_);++i)
		SAFE_DELETE(state_[i]);
	SAFE_DELETE(pCollider_);
}

void BigWhiteSkel::Update(Matrix V, Matrix P)
{
	EffectUpdate(V, P);
	if (IsActive() == false)
		return;
	// IDLE, RUN, ATTACK(RUN 0 번 재탕으로 뛰어오르기)
	StateUpdate();	// 참고로, 얘는 Attack 할때는 중력과 땅검사 안함.
	__super::Move(moveAmount);
	// GroundCheck 를 한정 조건화 함. ( Attack 중에는 땅검사 안함 )
	// Gravity도 마찬가지

	// SetPlay를 각 상태쪽으로 넘김.
	_animation->SetPosition(GetPosition());
	_animation->SetRotation(GetRotation());
	_animation->Update(V, P);

	pCollider_->SetPosition(GetPosition());
	pCollider_->SetRotation(GetRotation());
	pCollider_->SetScale(_animation->GetTextureRealSize());
	pCollider_->SetPivot(_animation->GetPivot());
	pCollider_->Update(V, P);
}

void BigWhiteSkel::Render()
{
	EffectRender();
	if (IsActive() == false)
		return;
	_animation->Render();
	pCollider_->Render();
}

void BigWhiteSkel::Reset()
{
	SetScale(3.0f * WSCALEX, 3.0f * WSCALEY);
	_animation->SetScale(GetScale());
	chaseTarget_ = (GameActor*)OBJECTMANAGER->FindObject("player");
//	this->SetPosition(chaseTarget_->GetPosition());	// 테스트코드
	actorData_.HP = 18;
	actorData_.maxHP = 18;
	actorData_.ImmuneTime = 0;
	actorData_.living = ActorState::LIVE;
	SetActive(true);
}

void BigWhiteSkel::StateUpdate()
{
	currentState_->SwitchState(*this);
	currentState_->Action(*this);
}

void BigWhiteSkel::AttackCheck()
{
	// 겹쳐져 있으면 공격 성공, 타겟의 무적시간이 0 이면서
	if (chaseTarget_->GetImmuneFrame() <= 0 && Collider::IntersectAABB(pCollider_, chaseTarget_->GetCollider()))
	{
		chaseTarget_->Attacked(actorData_.damage);
	}
}

void BigWhiteSkel::Jump()
{
	Vector2 tempPosition = GetPosition();
	float dX = chaseTarget_->GetPosition().x - GetPosition().x;
	float sign = copysign(1, dX);	// copysign 부호 비트 추출 어쨋든 양수면 1, 음수면 -1
	moveAmount.x = sign * moveSpeed * TIMEMANAGER->Delta();	// Next: -1 곱해야 할 수 있음.

	sign -= 1.0f;	// 양수 0, 음수 -2
	SetRotation(0.0f, sign * 90.0f, 0.0f);

	if (isGround_ == true) {
		isJump = true;
		isGround_ = false;
		moveAmount.y = jumpSpeed * 0.07f;
	}
}

void BigWhiteSkel::Move()
{
	// chaseTarget_ 추적
	float dX = chaseTarget_->GetPosition().x - GetPosition().x;
	float sign = copysign(1, dX);	// copysign 부호 비트 추출 어쨋든 양수면 1, 음수면 -1
	moveAmount.x = sign * moveSpeed * TIMEMANAGER->Delta();		// Next: -1 곱해야 할 수 있음.

	sign -= 1.0f;	// 양수 0, 음수 -2
	SetRotation(0.0f, sign * 90.0f, 0.0f);
}

void BigWhiteSkel::Idle()
{
	moveAmount.x = 0.0f;
	// 하는거 없어서 할당 안함.
}
// 애니메이션 자체는 Move와 Jump의 조합.
void BigWhiteSkel::Attack()
{
	AttackCheck();
}