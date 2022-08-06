#include "ImitationGame/framework.h"
#include "Physics/Collider.h"
#include "State/State.h"
#include "State/LittleGhostState/LittleGhostState.h"
#include "State/LittleGhostState/IdleStateLG.h"
#include "State/LittleGhostState/MoveStateLG.h"
#include "State/LittleGhostState/AttackStateLG.h"
#include "LittleGhost.h"

LittleGhost::LittleGhost()
{
	SetActive(false);
	state_[0] = new MoveStateLG();
	state_[1] = new AttackStateLG();
	currentState_ = state_[static_cast<int>(stateEnum_)];

	wstring strImage = IMAGE_FOLDER; strImage += L"Monster/LittleGhost/LittleGhost0.png";
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";

	_animation = new Animation(strImage, strShader);

	// MOVE
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);

		for (int i = 0; i <= 5; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Monster/LittleGhost/LittleGhost" + to_wstring(i) + L".png";
			pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 20, 20, 0.06f);

		}
		_animation->AddClip(pClip);
	}
	// Attack
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);

		for (int i = 0; i <= 2; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Monster/LittleGhost/LittleGhostAttack" + to_wstring(i) + L".png";
			pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 20, 20, 0.06f);
		}
		_animation->AddClip(pClip);
	}
	// 배율 설정 및 충돌체 할당
	pCollider_ = new Collider();
}

LittleGhost::~LittleGhost()
{
	SAFE_DELETE(_animation);
	SAFE_DELETE(currentState_);
	for (int i = 0; i < _countof(state_); ++i)
		SAFE_DELETE(state_[i]);
	SAFE_DELETE(pCollider_);
}

void LittleGhost::Update(Matrix V, Matrix P)
{	
	EffectUpdate(V, P);
	if (IsActive() == false)
		return;
	// IDLE, RUN, ATTACK(RUN 0 번 재탕으로 뛰어오르기)
	StateUpdate();
	// GroundCheck 를 한정 조건화 함. ( Attack 중에는 땅검사 안함 )
	// Gravity도 마찬가지 ( 지금 구현하는 LittleGhost 처럼 충돌이 발생하지 않는 애들도 있어서 )

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

void LittleGhost::Render()
{
	EffectRender();
	if (IsActive() == false)
		return;
	_animation->Render();
	pCollider_->Render();
}

void LittleGhost::Reset()
{
	SetScale(3.0f * WSCALEX, 3.0f * WSCALEY);
	_animation->SetScale(GetScale());
	chaseTarget_ = (GameActor*)OBJECTMANAGER->FindObject("player");
	currentState_->Enter(*this);
	actorData_.HP = 8;
	actorData_.maxHP = 8;
	actorData_.ImmuneTime = 0;
	actorData_.armor = 0;
	actorData_.living = ActorState::LIVE;
	SetActive(true);
}

void LittleGhost::StateUpdate()
{
	currentState_->SwitchState(*this);
	currentState_->Action(*this);
}

void LittleGhost::AttackCheck()
{
	// 겹쳐져 있으면 공격 성공, 타겟의 무적시간이 0 이면서
	if (chaseTarget_->GetImmuneFrame() <= 0 && Collider::IntersectAABB(pCollider_, chaseTarget_->GetCollider()))
	{
		chaseTarget_->Attacked(actorData_.damage);
	}
}

void LittleGhost::Move(float& radian)
{
	// chaseTarget_ 추적
	float sign = copysign(1, radian);	// copysign 부호 비트 추출 어쨋든 양수면 1, 음수면 -1
	ModifyPosition(cosf(radian) * moveSpeed * TIMEMANAGER->Delta(),
		sinf(radian) * moveSpeed * TIMEMANAGER->Delta());	// Next: -1 곱해야 할 수 있음.

	// 각도는 정상
	sign -= 1.0f;	// 양수 2, 음수 0
	SetRotation(0.0f, sign * 90.0f, 0.0f);
}

void LittleGhost::Move()
{
	printf("LittleGhost는 distance를 받는 개별 Move를 사용함");
}

void LittleGhost::Attack()
{
	printf("LittleGhost는 dX를 받는 개별 Attack을 사용함");
}

// 점프와 같지만 dX는 딱 한번만 연산함.
void LittleGhost::Attack(float& radian, float& dX, float& dY)
{	
	ModifyPosition(cosf(radian) * moveSpeed * TIMEMANAGER->Delta(),
		sinf(radian) * moveSpeed * TIMEMANAGER->Delta());	// Next: -1 곱해야 할 수 있음.

	AttackCheck();
}
void LittleGhost::Dying()
{
	Audio->Play("Explosion");
	actorData_.living = ActorState::DYING;
	dieEffect_ = objectPool->GetMonsterDieEffect();
	(*dieEffect_)->SetPlay(0);
	(*dieEffect_)->SetPosition(GetPosition());
	(*dieEffect_)->SetScale(GetScale());
}