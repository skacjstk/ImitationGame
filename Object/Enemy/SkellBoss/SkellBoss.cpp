#include "ImitationGame/framework.h"
#include "Physics/Collider.h"
#include "State/State.h"/*
#include "State/SkellBossState/SkellBossState.h"
#include "State/SkellBossState/IdleState.h"
#include "State/SkellBossState/RunState.h"
#include "State/SkellBossState/AttackState.h"
#include "State/SkellBossState/JumpState.h"
#include "State/SkellBossState/WaitState.h"
*/
#include "SkellBoss.h"

SkellBoss::SkellBoss()
{
	SetActive(false);	

	wstring strImage = IMAGE_FOLDER; strImage += L"Monster/SkellBoss/SkellBossT.png";
	wstring strShader = SHADER_FOLDER; strShader += L"TextureColor.hlsl";

	_animation = new Animation(strImage, strShader);

	// IDLE
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);

		for (int i = 0; i <= 9; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Monster/SkellBoss/Head/Idle/SkellBossIdle" + to_wstring(i) + L".png";
			pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.06f);

		}
		_animation->AddClip(pClip);
	}
	// Attack ( Bullet )
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);

		for (int i = 0; i <= 9; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Monster/SkellBoss/Head/Attack/SkellBossAttack0" + to_wstring(i) + L".png";
			pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);

		}
		_animation->AddClip(pClip);
	}
	// DYING 1������	// IDLE 0�� ����
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		strImage = IMAGE_FOLDER; strImage += L"Monster/SkellBoss/Head/Idle/SkellBossIdle0.png";
		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);		
		_animation->AddClip(pClip);
	}
	// DIE	 1 ������
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		strImage = IMAGE_FOLDER; strImage += L"Monster/SkellBoss/Head/Die/SkellBossDead_0.png";
		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);
		_animation->AddClip(pClip);
	}

	// �¿� �� ����
	strImage = IMAGE_FOLDER; strImage += L"Monster/SkellBoss/SkellBossLeftHandIdle0.png";
	hand_[0] = new Animation(strImage, strShader);
	hand_[1] = new Animation(strImage, strShader);
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);
		AnimationClip* pClip2 = new AnimationClip(AnimationClip::eState::Loop);

		for (int i = 0; i <= 9; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Monster/SkellBoss/SkellBossLeftHandIdle" + to_wstring(i) + L".png";
			pClip->AddFrame(hand_[0]->GetTexture(), strImage, 0, 0, 0.06f);
			pClip2->AddFrame(hand_[1]->GetTexture(), strImage, 0, 0, 0.06f);

		}
		hand_[0]->AddClip(pClip);
		hand_[1]->AddClip(pClip2);
	}
	// Attack
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::End);
		AnimationClip* pClip2 = new AnimationClip(AnimationClip::eState::End);

		for (int i = 0; i <= 17; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Monster/SkellBoss/SkellBossLeftHandAttack" + to_wstring(i / 10) + to_wstring(i % 10) + L".png";
			pClip->AddFrame(hand_[0]->GetTexture(), strImage, 0, 0, 0.06f);
			pClip2->AddFrame(hand_[1]->GetTexture(), strImage, 0, 0, 0.06f);

		}
		hand_[0]->AddClip(pClip);
		hand_[1]->AddClip(pClip2);
	}
	pCollider_ = new Collider();
}

SkellBoss::~SkellBoss()
{
	SAFE_DELETE(_animation);
	SAFE_DELETE(pCollider_);
}

void SkellBoss::Update(Matrix V, Matrix P)
{
	if (IsActive() == false)
		return;
	// IDLE, RUN, ATTACK(RUN 0 �� �������� �پ������)
	SwitchState();
	Action(V, P);

	// ���γ���
}

void SkellBoss::Render()
{	
	EffectRender();
	if (IsActive() == false)
		return;
	_animation->Render();
	hand_[0]->Render();
	hand_[1]->Render();
	pCollider_->Render();
}

void SkellBoss::Reset()
{
	SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
	_animation->SetScale(GetScale());
	pCollider_->SetScale(_animation->GetTextureRealSize());
	hand_[0]->SetScale(GetScale());
	hand_[1]->SetScale(GetScale());

	chaseTarget_ = (GameActor*)OBJECTMANAGER->FindObject("player");
	actorData_.HP = 100;
	actorData_.maxHP = 100;
	actorData_.ImmuneTime = 0;
	actorData_.living = ActorState::LIVE;

	stateEnum_ = eSkellBossState::HIDE;
	SwitchState = std::bind(&SkellBoss::SwitchStateHIDE, this);
	Action = std::bind(&SkellBoss::ActionHIDE, this, std::placeholders::_1, std::placeholders::_2);
	Enter = std::bind(&SkellBoss::EnterHIDE, this);
	_animation->SetPosition(GetPosition());
	_animation->SetRotation(GetRotation());

	Vector2 handPos = GetPosition();
	handGap = Vector2(600.0f * WSCALEX, 200.0f * WSCALEY);
	hand_[0]->SetPosition(handPos.x - handGap.x, handPos.y - handGap.y);	// �޼�
	hand_[1]->SetPosition(handPos.x + handGap.x, handPos.y + handGap.y);	// ������

	hand_[1]->SetRotation(0.0f, 180.0f, 0.0f);	// �̹����� �޼� ������

	pCollider_->SetPosition(GetPosition());
	pCollider_->SetRotation(GetRotation());
	SetActive(true);
}

bool SkellBoss::CheckPlayer()
{	
	if (fabsf(this->_position.x - chaseTarget_->GetPosition().x) <= (40.0f * WSCALEX)) {
		return true;
	}
	else
		return false;
}
/// <summary>
/// HIDE ���� ////////////////////////////////////////////
/// </summary>
void SkellBoss::SwitchStateHIDE()
{
	// �÷��̾� �Ÿ�����: ���� ������ ���, APPEAR ��ȯ
	if(CheckPlayer() == true){
	stateEnum_ = eSkellBossState::APPEAR;
	SwitchState = std::bind(&SkellBoss::SwitchStateAPPEAR, this);
	Action = std::bind(&SkellBoss::ActionAPPEAR, this, std::placeholders::_1, std::placeholders::_2);
	Enter = std::bind(&SkellBoss::EnterAPPEAR, this);
	Enter();
	}
}

void SkellBoss::EnterHIDE()
{
//	_animation->GetTexture()->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, 0.0f), 11);
	_animation->SetPosition(GetPosition());
	_animation->SetRotation(GetRotation());

	pCollider_->SetPosition(GetPosition());
	pCollider_->SetRotation(GetRotation());

	Vector2 handPos = GetPosition();
	hand_[0]->SetPosition(handPos.x - handGap.x, handPos.y - handGap.y);	// �޼�
	hand_[1]->SetPosition(handPos.x + handGap.x, handPos.y + handGap.y);	// ������
}
void SkellBoss::ActionHIDE(Matrix V, Matrix P)
{
	// Update�� ���� �ʰ� Render �ϸ� ���װ� ����?
}
//END HIDE
/// <summary>
/// APPEAR ���� ////////////////////////////////////////////
/// </summary>
void SkellBoss::SwitchStateAPPEAR()
{
	// 1�ʰ� ���� ������ ����
	if ((1.0f - appearAlpha) < 0.001f) {
	//	MessageBoxW(nullptr, L"WAIT ����", L"SkellBoss", MB_OK);
		stateEnum_ = eSkellBossState::WAIT;
		SwitchState = std::bind(&SkellBoss::SwitchStateWAIT, this);
		Action = std::bind(&SkellBoss::ActionWAIT, this, std::placeholders::_1, std::placeholders::_2);
		Enter = std::bind(&SkellBoss::EnterWAIT, this);
		_animation->GetTexture()->UpdateColorBuffer(0);
		CAMERA->SetObject(chaseTarget_);
		Enter();
	}
}


void SkellBoss::EnterAPPEAR()
{
	_animation->SetPlay(0);	// IDLE
	hand_[0]->SetPlay(0);
	hand_[1]->SetPlay(0);
	// Next: ��� ��ġ ���;� ��.
	CAMERA->SetObject(this);
	Audio->Play("JailBoss");	// �����: ��� ���
	Audio->Play("Blaugh");	// ���¼Ҹ�: �ѹ� ���
}


void SkellBoss::ActionAPPEAR(Matrix V, Matrix P)
{
	appearAlpha = min(appearAlpha + TIMEMANAGER->Delta() * 0.5f, 1.0f);	// 1.0 �̻� �ȿö󰡰�
	_animation->GetTexture()->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, appearAlpha), 11);	// ���İ��� ����Ǵ� ���̴�
	hand_[0]->GetTexture()->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, appearAlpha), 11);
	hand_[1]->GetTexture()->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, appearAlpha), 11);
	_animation->Update(V, P);
	hand_[0]->Update(V, P);
	hand_[1]->Update(V, P);
		// �浹ü ������ �ʿ� ����.

	// Next: �¿� �յ� �̷��� �ٲٱ�
}
/// <summary>
/// WAIT ���� ////////////////////////////////////////////
/// </summary>
void SkellBoss::SwitchStateWAIT()
{
	waitCycle_ += TIMEMANAGER->Delta();
	if (waitCycle_ >= 2.0f) {
		// 3���� ���ݻ��� �� �ϳ��� ����
	}
}
void SkellBoss::EnterWAIT()
{
	_animation->SetPosition(GetPosition());
	_animation->SetRotation(GetRotation());

	pCollider_->SetPosition(GetPosition());
	pCollider_->SetRotation(GetRotation());

	Vector2 handPos = GetPosition();
	hand_[0]->SetPosition(handPos.x - handGap.x, handPos.y - handGap.y);	// �޼�
	hand_[1]->SetPosition(handPos.x + handGap.x, handPos.y + handGap.y);	// ������
}

void SkellBoss::ActionWAIT(Matrix V, Matrix P)
{
	_animation->Update(V, P);
	hand_[0]->Update(V, P);
	hand_[1]->Update(V, P);
	pCollider_->Update(V, P);
}