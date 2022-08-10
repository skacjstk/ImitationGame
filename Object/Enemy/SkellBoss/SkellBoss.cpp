#include "ImitationGame/framework.h"
#include "Physics/Collider.h"
#include "State/State.h"
#include "Object/Bullet/BossBullet.h"
#include "Object/Bullet/BossSword.h"
#include "Object/Bullet/BossLaser.h"
#include "SkellBoss.h"

void SkellBoss::GenerateBullet()
{
	for (UINT i = 0; i < _countof(bullets_); ++i) {
		bullets_[i] = new BossBullet();	// 18 발 쏠꺼임 10도 옮겨가면서
	}
	for (UINT i = 0; i < _countof(swords_); ++i)
		swords_[i] = new BossSword();
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
	// 보스소드도 여기서 만듬
	thread t(std::bind(&SkellBoss::GenerateBullet, this));
	t.detach();
	// 레이저는 2개밖에 안되고, 2개가 좌우구분이라 귀찮아서 상시 렌더링하거든? 그래서 쓰레드로 빼면 안돼
	for (UINT i = 0; i < _countof(lasers_); ++i)
		lasers_[i] = new BossLaser();
	// 쓰레드화 하기  240개라 오래걸림
	myPointer = this;
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
}

void SkellBoss::Render()
{	
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
	for (int i = 0; i < numOfActiveSwords_; ++i) {
		swords_[i]->Render();
	}
	for (UINT i = 0; i < _countof(lasers_); ++i) {
		lasers_[i]->Render();
	}
}

void SkellBoss::Reset()
{
	SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
	_animation->SetScale(GetScale());
	pCollider_->SetScale(_animation->GetTextureRealSize());	// Wait, Hide 때도 한번씩 변화함
	hand_[0]->SetScale(GetScale());
	hand_[1]->SetScale(GetScale());

	chaseTarget_ = (GameActor*)OBJECTMANAGER->FindObject("player");
	actorData_.HP = 100;
	actorData_.maxHP = 100;
	actorData_.ImmuneTime = 0;
	actorData_.living = ActorState::LIVE;
	actorData_.type = ActorType::Enemy;
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
	Enter();	// HIDE로 진입함
	SetActive(true);
}

bool SkellBoss::UpdateBulletCycle(int divFrame)
{
	++bulletCycle_;
	if (bulletCycle_ >= (TIMEMANAGER->GetFrame() / divFrame))
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
	pCollider_->SetScale(0.0f,0.0f);

	Vector2 handPos = GetPosition();
	hand_[0]->SetPosition(handPos.x - handGap.x, handPos.y - handGap.y);	// 왼손
	hand_[1]->SetPosition(handPos.x + handGap.x, handPos.y + handGap.y);	// 오른손

	back_->SetPosition(GetPosition());
	back_->SetScale(GetScale());
	back_->ModifyPosition(8.0f * GetScale().x, -24.0f * GetScale().y);
}
void SkellBoss::ActionHIDE(Matrix V, Matrix P)
{
	pCollider_->Update(V, P);	// 숨어있을땐 충돌 불가능하게
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
	pCollider_->SetScale(_animation->GetTextureRealSize());
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

	if (waitCycle_ >= selectPatternWaitTime_) {
		// 3개의 공격상태 중 하나로 변경
		waitCycle_ = 0.0f;
		std::random_device rd;
		std::default_random_engine eng(rd());
		std::uniform_int_distribution<> distr(0, 2);
		
		switch (distr(eng)) {
		case 0:
			stateEnum_ = eSkellBossState::BULLET;
			SwitchState = std::bind(&SkellBoss::SwitchStateBULLET, this);
			Action = std::bind(&SkellBoss::ActionBULLET, this, std::placeholders::_1, std::placeholders::_2);
			Enter = std::bind(&SkellBoss::EnterBULLET, this);
			break;
		case 1:
			stateEnum_ = eSkellBossState::SWORD;
			SwitchState = std::bind(&SkellBoss::SwitchStateSWORD, this);
			Action = std::bind(&SkellBoss::ActionSWORD, this, std::placeholders::_1, std::placeholders::_2);
			Enter = std::bind(&SkellBoss::EnterSWORD, this);
			break;
		case 2:
			stateEnum_ = eSkellBossState::LASER;
			SwitchState = std::bind(&SkellBoss::SwitchStateLASER, this);
			Action = std::bind(&SkellBoss::ActionLASER, this, std::placeholders::_1, std::placeholders::_2);
			Enter = std::bind(&SkellBoss::EnterLASER, this);
			break;
		}
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
//	hand_[0]->SetPosition(handPos.x - handGap.x, handPos.y - handGap.y);	// 왼손
//	hand_[1]->SetPosition(handPos.x + handGap.x, handPos.y + handGap.y);	// 오른손
	// Laser 추가로 Wait 시점에 초기화 할 필요 없어졌음.
	hand_[0]->SetPlay(0);
	hand_[1]->SetPlay(0);
}

void SkellBoss::ActionWAIT(Matrix V, Matrix P)
{

	if (inactiveIndex < numOfActiveBullets_ && UpdateBulletCycle(8))
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
	if (UpdateBulletCycle(9) && (numOfActiveBullets_ / 4) < (_countof(bullets_) / 4)) {
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

void SkellBoss::SwitchStateSWORD()
{	
	// 칼이 죽을때마다 Event를 호출하고,0이 되면 Wait로 바뀌게 함.
	if (numOfInactiveSwords_ <= 0) {
		stateEnum_ = eSkellBossState::WAIT;
		SwitchState = std::bind(&SkellBoss::SwitchStateWAIT, this);
		Action = std::bind(&SkellBoss::ActionWAIT, this, std::placeholders::_1, std::placeholders::_2);
		Enter = std::bind(&SkellBoss::EnterWAIT, this);
		Enter();
	}
}

void SkellBoss::EnterSWORD()
{
	_animation->SetPlay(0);
	bulletCycle_ = 0;
	bulletRadian_ = 0.0f;
	numOfActiveSwords_ = 0;
	numOfInactiveSwords_ = 6;	// 칼이 죽을때마다 Event를 호출하고,0이 되면 Wait로 바뀌게 함.
}

void SkellBoss::ActionSWORD(Matrix V, Matrix P)
{
	// 최초 생성 시기를 Bullet과 공유함.
	Vector2 summonPos = this->_position;
	summonPos.y += 300.0f * WSCALEY;
	if (UpdateBulletCycle(9) && numOfActiveSwords_ < _countof(swords_)) {
		summonPos.x += (-600.0f * WSCALEX) + 200.0f * WSCALEX * numOfActiveSwords_;
		swords_[numOfActiveSwords_]->Reset();
		swords_[numOfActiveSwords_]->SummonSword(&myPointer, summonPos, 1.5f + ((float)numOfActiveSwords_ * 0.15f), chaseTarget_);
		++numOfActiveSwords_;
		Audio->Play("SpawnMonster");
	}
	
	for (int i = 0; i < numOfActiveSwords_; ++i) {
		swords_[i]->Update(V, P);
	}
	_animation->Update(V, P);
	hand_[0]->Update(V, P);
	hand_[1]->Update(V, P);
	back_->Update(V, P);
	pCollider_->Update(V, P);
}

void SkellBoss::SwitchStateLASER()
{
	// endLaser
	if (endLaser == true) {
		stateEnum_ = eSkellBossState::WAIT;
		SwitchState = std::bind(&SkellBoss::SwitchStateWAIT, this);
		Action = std::bind(&SkellBoss::ActionWAIT, this, std::placeholders::_1, std::placeholders::_2);
		Enter = std::bind(&SkellBoss::EnterWAIT, this);
		Enter();
	}
}

void SkellBoss::EnterLASER()
{
	bulletCycle_ = 0;
	bulletRadian_ = 0.0f;
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<> distr(minLaserCount_, maxLaserCount_);
	remainLaserCount_ = FireLaserCount_ = distr(eng);
	// 일정 수 사이
	std::uniform_int_distribution<> distr2(0, 1);
	laserDirection_ = distr2(eng);	// 0은 왼쪽발사 시작, 1은 오른쪽 발사 시작
	// 시작이 왼쪽이면 오른손이 움직여서 "왼쪽"으로 발사해야 함
	endLaser = false;
}

void SkellBoss::ActionLASER(Matrix V, Matrix P)
{
	// 이 주기 마다
	if (UpdateBulletCycle(1) && FireLaserCount_ > 0) {
		if (laserDirection_ == 0)
			laserDirection_ = 1;
		else
			laserDirection_ = 0;

		hand_[laserDirection_]->SetPosition(hand_[laserDirection_]->GetPosition().x, chaseTarget_->GetPosition().y);
		// 추적이 완료되면, 발사애니메이션 재생
		hand_[laserDirection_]->SetPlay(1, true);
		--FireLaserCount_;
	//	Audio->Play("BossLaser");		// 사운드 호출 시점을 BossLaser SummonLaser로 옮김. 원본사운드 찾으면 다시 옮길수도 있음.
	}	
	//	Attack이 Hand의 1번 클립 이 8번 클립일 때
	if (hand_[laserDirection_]->GetAnimationClip(1)->GetCurrentFrame() == 9 && beforeShootLaser == false) {
		// 애니메이션 번호에 맞춰 레이저 소환
		lasers_[laserDirection_]->Reset();
		lasers_[laserDirection_]->SummonLaser(&myPointer, hand_[laserDirection_]->GetPosition(),
			static_cast<BossLaser::HandDirection>(laserDirection_), chaseTarget_);
		beforeShootLaser = true;
	}
	else {
		beforeShootLaser = false;
	}
	lasers_[0]->Update(V, P);
	lasers_[1]->Update(V, P);

	_animation->Update(V, P);
	hand_[0]->Update(V, P);
	hand_[1]->Update(V, P);
	back_->Update(V, P);
	pCollider_->Update(V, P);
}
