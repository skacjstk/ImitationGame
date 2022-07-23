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

		{35.0f,31.0f, 0, 1.0f	* -1.5f * WSCALEY},
		{36.0f,31.0f, 0, 1.0f	* -1.5f * WSCALEY},
		{61.0f,48.0f, 0, 18.0f	* -1.5f * WSCALEY},
		{57.0f,48.0f, 0, 18.0f	* -1.5f * WSCALEY},
		{58.0f,40.0f, 0, 10.0f	* -1.5f * WSCALEY},
		{58.0f,26.0f, 0, -4.0f	* -1.5f * WSCALEY},
		{45.0f,26.0f, 0, -4.0f 	* -1.5f * WSCALEY},
		{51.0f,30.0f, 0, 0.0f	* -1.5f * WSCALEY},
		{54.0f,48.0f, 0, 18.0f	* -1.5f * WSCALEY},
		{65.0f,48.0f, 0, 18.0f	* -1.5f * WSCALEY},
		{62.0f,48.0f, 0, 18.0f	* -1.5f * WSCALEY},
		{36.0f,34.0f, 0, 4.0f	* -1.5f * WSCALEY},
		{33.0f,30.0f, 0, 0.0f}
		
		
		};
		for (int i = 0; i <= 12; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Monster/BigWhiteSkel/BigWhiteSkelAttack" + to_wstring((int)(i * 0.1)) + to_wstring(i % 10) + L".png";
			pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, temp[i][0], temp[i][1], 0.1f, temp[i][2], temp[i][3]);
	//		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);
		}
		_animation->AddClip(pClip);
	}
	// JUMP	// Idle 0�� ����
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		strImage = IMAGE_FOLDER; strImage += L"Monster/BigWhiteSkel/BigWhiteSkelIdle.png";
		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);
		_animation->AddClip(pClip);
	}
	// Wait	// IDLE 0�� ����
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		strImage = IMAGE_FOLDER; strImage += L"Monster/BigWhiteSkel/BigWhiteSkelIdle.png";
		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);
		_animation->AddClip(pClip);
	}
	// ���� ���� �� �浹ü �Ҵ�
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
	if (IsActive() == false)
		return;
	// IDLE, RUN, ATTACK(RUN 0 �� �������� �پ������)
	StateUpdate();
	// GroundCheck �� ���� ����ȭ ��. ( Attack �߿��� ���˻� ���� )
	// Gravity�� ��������

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

void BigWhiteSkel::Render()
{
	if (IsActive() == false)
		return;
	_animation->Render();
	pCollider_->Render();
}

void BigWhiteSkel::Reset()
{
	SetActive(true);
	SetScale(3.0f * WSCALEX, 3.0f * WSCALEY);
	_animation->SetScale(GetScale());
	chaseTarget_ = (GameActor*)OBJECTMANAGER->FindObject("player");
	this->SetPosition(chaseTarget_->GetPosition());
}

void BigWhiteSkel::StateUpdate()
{
	currentState_->SwitchState(*this);
	currentState_->Action(*this);
}

void BigWhiteSkel::AttackCheck()
{
	// ������ ������ ���� ����, Ÿ���� �����ð��� 0 �̸鼭
	if (chaseTarget_->GetImmuneFrame() <= 0 && Collider::IntersectAABB(pCollider_, chaseTarget_->GetCollider()))
	{
		chaseTarget_->Attacked();
	}
}

void BigWhiteSkel::Jump()
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
		gravity_ = jumpSpeed * 0.07f;
	}
}

void BigWhiteSkel::Move()
{
	// chaseTarget_ ����
	float dX = chaseTarget_->GetPosition().x - GetPosition().x;
	float sign = copysign(1, dX);	// copysign ��ȣ ��Ʈ ���� ��¶�� ����� 1, ������ -1
	ModifyPosition(sign * moveSpeed * TIMEMANAGER->Delta(), 0.0f);	// Next: -1 ���ؾ� �� �� ����.

	sign -= 1.0f;	// ��� 0, ���� -2
	SetRotation(0.0f, sign * 90.0f, 0.0f);
}

void BigWhiteSkel::Idle()
{
	// �ϴ°� ��� �Ҵ� ����.
}
// �ִϸ��̼� ��ü�� Move�� Jump�� ����.
void BigWhiteSkel::Attack()
{
	printf("BigWhiteSkel�� dX�� �޴� ���� Attack�� �����");
}

// ������ ������ dX�� �� �ѹ��� ������.
void BigWhiteSkel::Attack(float& dX)
{
	AttackCheck();
};