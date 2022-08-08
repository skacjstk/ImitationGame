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

	wstring strImage = IMAGE_FOLDER; strImage += L"Boss/SkellBoss/SkellBossT.png";
	wstring strShader = SHADER_FOLDER; strShader += L"TextureColor.hlsl";

	_animation = new Animation(strImage, strShader);

	// IDLE
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);

		for (int i = 0; i <= 9; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Boss/SkellBoss/Head/Idle/SkellBossIdle" + to_wstring(i) + L".png";
			pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.06f);

		}
		_animation->AddClip(pClip);
	}
	// Attack ( Bullet )
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);

		for (int i = 0; i <= 9; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Boss/SkellBoss/Head/Attack/SkellBossAttack0" + to_wstring(i) + L".png";
			pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);

		}
		_animation->AddClip(pClip);
	}
	// DYING 1프레임	// IDLE 0번 재탕
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		strImage = IMAGE_FOLDER; strImage += L"Boss/SkellBoss/Head/Idle/SkellBossIdle0.png";
		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);		
		_animation->AddClip(pClip);
	}
	// DIE	 1 프레임
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		strImage = IMAGE_FOLDER; strImage += L"Boss/SkellBoss/Head/Die/SkellBossDead_0.png";
		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);
		_animation->AddClip(pClip);
	}

	// 좌우 손 설정
	strImage = IMAGE_FOLDER; strImage += L"Boss/SkellBoss/SkellBossLeftHandIdle0.png";
	hand_[0] = new Animation(strImage, strShader);
	hand_[1] = new Animation(strImage, strShader);
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);
		AnimationClip* pClip2 = new AnimationClip(AnimationClip::eState::Loop);

		for (int i = 0; i <= 9; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Boss/SkellBoss/SkellBossLeftHandIdle" + to_wstring(i) + L".png";
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
			strImage = IMAGE_FOLDER; strImage += L"Boss/SkellBoss/SkellBossLeftHandAttack" + to_wstring(i / 10) + to_wstring(i % 10) + L".png";
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
	// IDLE, RUN, ATTACK(RUN 0 번 재탕으로 뛰어오르기)
	SwitchState();
	Action(V, P);

	// 따로넣자
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
	hand_[0]->SetPosition(handPos.x - handGap.x, handPos.y - handGap.y);	// 왼손
	hand_[1]->SetPosition(handPos.x + handGap.x, handPos.y + handGap.y);	// 오른손

	hand_[1]->SetRotation(0.0f, 180.0f, 0.0f);	// 이미지가 왼손 기준임

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
/// HIDE 상태 ////////////////////////////////////////////
/// </summary>
void SkellBoss::SwitchStateHIDE()
{
	// 플레이어 거리측정: 일정 이하일 경우, APPEAR 전환
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
	_animation->SetPosition(GetPosition());
	_animation->SetRotation(GetRotation());

	pCollider_->SetPosition(GetPosition());
	pCollider_->SetRotation(GetRotation());

	Vector2 handPos = GetPosition();
	hand_[0]->SetPosition(handPos.x - handGap.x, handPos.y - handGap.y);	// 왼손
	hand_[1]->SetPosition(handPos.x + handGap.x, handPos.y + handGap.y);	// 오른손
}
void SkellBoss::ActionHIDE(Matrix V, Matrix P)
{
	// Update를 하지 않고 Render 하면 버그가 날까?
}
//END HIDE
/// <summary>
/// APPEAR 상태 ////////////////////////////////////////////
/// </summary>
void SkellBoss::SwitchStateAPPEAR()
{
	// alpha값이 활성화가 다 되면
	if ((1.0f - appearAlpha) < 0.001f) {
	//	MessageBoxW(nullptr, L"WAIT 변경", L"SkellBoss", MB_OK);
		stateEnum_ = eSkellBossState::WAIT;
		SwitchState = std::bind(&SkellBoss::SwitchStateWAIT, this);
		Action = std::bind(&SkellBoss::ActionWAIT, this, std::placeholders::_1, std::placeholders::_2);
		Enter = std::bind(&SkellBoss::EnterWAIT, this);
		_animation->GetTexture()->UpdateColorBuffer(0);
		hand_[0]->GetTexture()->UpdateColorBuffer(0);
		hand_[1]->GetTexture()->UpdateColorBuffer(0);
		CAMERA->SetObject(chaseTarget_);
		Enter();
	}
}


void SkellBoss::EnterAPPEAR()
{
	_animation->SetPlay(0);	// IDLE
	hand_[0]->SetPlay(0);
	hand_[1]->SetPlay(0);
	// Next: 양손 위치 나와야 함.
	CAMERA->SetObject(this);
	Audio->Play("JailBoss");	// 배경음: 계속 재생
	Audio->Play("Blaugh");	// 웃는소리: 한번 재생
}


void SkellBoss::ActionAPPEAR(Matrix V, Matrix P)
{
	appearAlpha = min(appearAlpha + TIMEMANAGER->Delta() * 0.5f, 1.0f);	// 1.0 이상 안올라가게
	_animation->GetTexture()->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, appearAlpha), 11);	// 알파값만 적용되는 셰이더
	hand_[0]->GetTexture()->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, appearAlpha), 11);
	hand_[1]->GetTexture()->UpdateColorBuffer(Color(0.0f, 0.0f, 0.0f, appearAlpha), 11);
	_animation->Update(V, P);
	hand_[0]->Update(V, P);
	hand_[1]->Update(V, P);
		// 충돌체 갱신할 필요 없음.

	// Next: 좌우 손도 이렇게 바꾸기
}
/// <summary>
/// WAIT 상태 ////////////////////////////////////////////
/// </summary>
void SkellBoss::SwitchStateWAIT()
{
	waitCycle_ += TIMEMANAGER->Delta();
	if (waitCycle_ >= 2.0f) {
		// 3개의 공격상태 중 하나로 변경
		MessageBoxW(MAIN->GetWindowHandler(), L"", L"", MB_OK);
		waitCycle_ = 0.0f;
	}
}
void SkellBoss::EnterWAIT()
{
	_animation->SetPosition(GetPosition());
	_animation->SetRotation(GetRotation());

	pCollider_->SetPosition(GetPosition());
	pCollider_->SetRotation(GetRotation());

	Vector2 handPos = GetPosition();
	hand_[0]->SetPosition(handPos.x - handGap.x, handPos.y - handGap.y);	// 왼손
	hand_[1]->SetPosition(handPos.x + handGap.x, handPos.y + handGap.y);	// 오른손
}

void SkellBoss::ActionWAIT(Matrix V, Matrix P)
{
	_animation->Update(V, P);
	hand_[0]->Update(V, P);
	hand_[1]->Update(V, P);
	pCollider_->Update(V, P);
}