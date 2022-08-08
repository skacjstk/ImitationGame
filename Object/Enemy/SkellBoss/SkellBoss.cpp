#include "ImitationGame/framework.h"
#include "Physics/Collider.h"
#include "State/State.h"
#include "Object/Bullet/BossBullet.h"
#include "SkellBoss.h"

void SkellBoss::GenerateBullet()
{
	for (UINT i = 0; i < _countof(bullets_); ++i) {
		bullets_[i] = new BossBullet();	// 18 발 쏠꺼임 10도 옮겨가면서
	}
}

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

	// 보스 배경
	strImage = IMAGE_FOLDER; strImage += L"Particle/SkellBossBack0.png";
	back_ = new Animation(strImage, strShader);
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);

		for (int i = 0; i <= 9; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Particle/SkellBossBack" + to_wstring(i) + L".png";
			pClip->AddFrame(back_->GetTexture(), strImage, 0, 0, 0.06f);

		}
		back_->AddClip(pClip);
	}
	pCollider_ = new Collider();
	thread t(std::bind(&SkellBoss::GenerateBullet, this));
	t.detach();
	// 쓰레드화 하기  240개라 오래걸림
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
	back_->Render();
	_animation->Render();
	hand_[0]->Render();
	hand_[1]->Render();
	pCollider_->Render();


	for (int i = 0; i < numOfActiveBullets_; ++i) {
		bullets_[i]->Render();
	}
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

	// 입에서 나오는 에너지 설정
	back_->SetPosition(GetPosition());
	back_->SetScale(GetScale());
	back_->ModifyPosition(7.0f * GetScale().x, -22.0f * GetScale().y);

	pCollider_->SetPosition(GetPosition());
	pCollider_->SetRotation(GetRotation());
	SetActive(true);
}

bool SkellBoss::UpdateBulletCycle()
{
	++bulletCycle_;
	if (bulletCycle_ >= (144 / 9))
	{
		bulletCycle_ = 0;
		return true;
	}
	return false;
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

	back_->SetPosition(GetPosition());
	back_->SetScale(GetScale());
	back_->ModifyPosition(8.0f * GetScale().x, -24.0f * GetScale().y);
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
		waitCycle_ = 0.0f;
		stateEnum_ = eSkellBossState::BULLET;
		SwitchState = std::bind(&SkellBoss::SwitchStateBULLET, this);
		Action = std::bind(&SkellBoss::ActionBULLET, this, std::placeholders::_1, std::placeholders::_2);
		Enter = std::bind(&SkellBoss::EnterBULLET, this);
		Enter();
	}
}
void SkellBoss::EnterWAIT()
{
	_animation->SetPlay(0);
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

	if (inactiveIndex < numOfActiveBullets_ && UpdateBulletCycle())
	{
		inactiveIndex += 4;
	}
	_animation->Update(V, P);
	back_->Update(V, P);
	hand_[0]->Update(V, P);
	hand_[1]->Update(V, P);
	pCollider_->Update(V, P);

	for (int i = numOfActiveBullets_ - 1; i >= inactiveIndex; --i) {
		bullets_[i]->Update(V, P);
	}
}

void SkellBoss::SwitchStateBULLET()
{
	if ((numOfActiveBullets_ / 4) >= (_countof(bullets_) / 4)) {
		stateEnum_ = eSkellBossState::WAIT;
		SwitchState = std::bind(&SkellBoss::SwitchStateWAIT, this);
		Action = std::bind(&SkellBoss::ActionWAIT, this, std::placeholders::_1, std::placeholders::_2);
		Enter = std::bind(&SkellBoss::EnterWAIT, this);
		Enter();
	}
}

void SkellBoss::EnterBULLET()
{
	_animation->SetPlay(1); 		
	bulletCycle_ = 0;
	inactiveIndex = 0;
	bulletRadian_ = 0.0f;
	numOfActiveBullets_ = 0;
	skelMousePos_ = back_->GetPosition();	// 총알상태 진입할때 skelMousePos 갱신
	if (bulletDirection_ > 0.0f)
		bulletDirection_ = -1.0f;
	else
		bulletDirection_ = 1.0f;
}

void SkellBoss::ActionBULLET(Matrix V, Matrix P)
{
	// TIMEMANAGER->GetFrame()
	// 144프레임 기준 14프레임, 60프레임 기준 6 프레임마다
	if (UpdateBulletCycle() && (numOfActiveBullets_ / 4) < (_countof(bullets_) / 4)) {
		// 기능 분리
		bulletRadian_ += PI * 0.04f * bulletDirection_;
		bullets_[numOfActiveBullets_]->Reset();
		bullets_[numOfActiveBullets_++]->Fire(bulletRadian_, skelMousePos_, chaseTarget_);	// reset을 대체하는 기능
		bullets_[numOfActiveBullets_]->Reset();
		bullets_[numOfActiveBullets_++]->Fire(bulletRadian_ + (PI * 0.5f), skelMousePos_, chaseTarget_);	// reset을 대체하는 기능
		bullets_[numOfActiveBullets_]->Reset();
		bullets_[numOfActiveBullets_++]->Fire(bulletRadian_ + PI, skelMousePos_, chaseTarget_);	// reset을 대체하는 기능
		bullets_[numOfActiveBullets_]->Reset();
		bullets_[numOfActiveBullets_++]->Fire(bulletRadian_ + (PI * 1.5f), skelMousePos_, chaseTarget_);	// reset을 대체하는 기능

		Audio->Play("BBullet");
	}

	// bullet 업데이트 시점 추가 (공용: 그러나 Wait 에서 active수 관리
	for (int i = 0; i < numOfActiveBullets_; ++i) {
		bullets_[i]->Update(V, P);
	}

	_animation->Update(V, P);
	hand_[0]->Update(V, P);
	hand_[1]->Update(V, P);
	back_->Update(V, P);
	pCollider_->Update(V, P);
}
