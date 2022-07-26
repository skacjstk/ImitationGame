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
	// ���� ���� �� �浹ü �Ҵ�
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
	if (IsActive() == false)
		return;
	// IDLE, RUN, ATTACK(RUN 0 �� �������� �پ������)
	StateUpdate();
	// GroundCheck �� ���� ����ȭ ��. ( Attack �߿��� ���˻� ���� )
	// Gravity�� �������� ( ���� �����ϴ� LittleGhost ó�� �浹�� �߻����� �ʴ� �ֵ鵵 �־ )

	// SetPlay�� �� ���������� �ѱ�.
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
	this->SetPosition(chaseTarget_->GetPosition());
	currentState_->Enter(*this);
	actorData_.HP = 12;
	actorData_.maxHP = 12;
	actorData_.ImmuneTime = 0;
	SetActive(true);
}

void LittleGhost::StateUpdate()
{
	currentState_->SwitchState(*this);
	currentState_->Action(*this);
}

void LittleGhost::AttackCheck()
{
	// ������ ������ ���� ����, Ÿ���� �����ð��� 0 �̸鼭
	if (chaseTarget_->GetImmuneFrame() <= 0 && Collider::IntersectAABB(pCollider_, chaseTarget_->GetCollider()))
	{
		chaseTarget_->Attacked(actorData_.damage);
	}
}

void LittleGhost::Move(float& radian)
{
	// chaseTarget_ ����
	float sign = copysign(1, radian);	// copysign ��ȣ ��Ʈ ���� ��¶�� ����� 1, ������ -1
	ModifyPosition(cosf(radian) * moveSpeed * TIMEMANAGER->Delta(),
		sinf(radian) * moveSpeed * TIMEMANAGER->Delta());	// Next: -1 ���ؾ� �� �� ����.

	// ������ ����
	sign -= 1.0f;	// ��� 2, ���� 0
	SetRotation(0.0f, sign * 90.0f, 0.0f);
}

void LittleGhost::Move()
{
	printf("LittleGhost�� distance�� �޴� ���� Move�� �����");
}

void LittleGhost::Attack()
{
	printf("LittleGhost�� dX�� �޴� ���� Attack�� �����");
}

// ������ ������ dX�� �� �ѹ��� ������.
void LittleGhost::Attack(float& radian, float& dX, float& dY)
{	
	ModifyPosition(cosf(radian) * moveSpeed * TIMEMANAGER->Delta(),
		sinf(radian) * moveSpeed * TIMEMANAGER->Delta());	// Next: -1 ���ؾ� �� �� ����.

	AttackCheck();
};