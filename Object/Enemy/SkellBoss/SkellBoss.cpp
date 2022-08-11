#include "ImitationGame/framework.h"
#include "Physics/Collider.h"
#include "State/State.h"
#include "GameUI/BossUI/BossLifeUI.h"
#include "Object/Bullet/BossBullet.h"
#include "Object/Bullet/BossSword.h"
#include "Object/Bullet/BossLaser.h"
#include "SkellBoss.h"

void SkellBoss::GenerateBullet()
{
	for (UINT i = 0; i < _countof(bullets_); ++i) {
		bullets_[i] = new BossBullet();	// 18 �� ���� 10�� �Űܰ��鼭
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
	// DYING 1������	// IDLE 0�� ����
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		strImage = IMAGE_FOLDER; strImage += L"Boss/SkellBoss/Head/Idle/SkellBossIdle0.png";
		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);		
		_animation->AddClip(pClip);
	}
	// DIE	 1 ������
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::EndStay);
		strImage = IMAGE_FOLDER; strImage += L"Boss/SkellBoss/Head/Die/SkellBossDead_0.png";
		pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.1f);
		_animation->AddClip(pClip);
	}

	// �¿� �� ����
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

	// ���� ���
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
	// �����ҵ嵵 ���⼭ ����
	thread t(std::bind(&SkellBoss::GenerateBullet, this));
	t.detach();
	// �������� 2���ۿ� �ȵǰ�, 2���� �¿챸���̶� �����Ƽ� ��� �������ϰŵ�? �׷��� ������� ���� �ȵ�
	for (UINT i = 0; i < _countof(lasers_); ++i)
		lasers_[i] = new BossLaser();
	// ������ȭ �ϱ�  240���� �����ɸ�
	myPointer = this;

	// UI �����ϱ�
	skellBossUI_ = new BossLifeUI();
}

SkellBoss::~SkellBoss()
{
	// ������ safe delete 
	for (UINT i = 0; i < _countof(dieEffect_); ++i) {
		if (dieEffect_ != nullptr) {
			(*dieEffect_[i])->GetAnimationClip(0)->SetTime(0.06f);
			dieEffect_[i] = nullptr;
		}
	}
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
}

void SkellBoss::Render()
{	
	if (IsActive() == false)
		return;
	else if (stateEnum_ == eSkellBossState::DYING)
	{
		back_->Render();
		_animation->Render();
		hand_[0]->Render();
		hand_[1]->Render();
		for (UINT i = 0; i < _countof(dieEffect_); ++i) 
			(*dieEffect_[i])->Render();
		return;
	}
	else if (stateEnum_ == eSkellBossState::DEAD) {
		_animation->Render();
		if(dieEffect_[0] != nullptr){
			for (UINT i = 0; i < _countof(dieEffect_); ++i)
				(*dieEffect_[i])->Render();
		}
		return;
	}

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
	skellBossUI_->Render();
}

void SkellBoss::Reset()
{
	SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
	_animation->SetScale(GetScale());
	pCollider_->SetScale(_animation->GetTextureRealSize());	// Wait, Hide ���� �ѹ��� ��ȭ��
	hand_[0]->SetScale(GetScale());
	hand_[1]->SetScale(GetScale());

	chaseTarget_ = (GameActor*)OBJECTMANAGER->FindObject("player");
	actorData_.HP = 1;
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
	hand_[0]->SetPosition(handPos.x - handGap.x, handPos.y - handGap.y);	// �޼�
	hand_[1]->SetPosition(handPos.x + handGap.x, handPos.y + handGap.y);	// ������
	hand_[1]->SetRotation(0.0f, 180.0f, 0.0f);	// �̹����� �޼� ������

	// �Կ��� ������ ������ ����
	back_->SetPosition(GetPosition());
	back_->SetScale(GetScale());
	back_->ModifyPosition(7.0f * GetScale().x, -22.0f * GetScale().y);

	pCollider_->SetPosition(GetPosition());
	pCollider_->SetRotation(GetRotation());

	//UI �����ϱ� y �ǾƷ� x �߾ӿ� ����.
	skellBossUI_->Reset();

	Enter();	// HIDE�� ������
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

void SkellBoss::Attacked(float damage)
{
	if (actorData_.living == ActorState::LIVE)
	{
		float changed = max(damage - actorData_.armor, 1.0f);
		actorData_.HP -= (int)changed;
		Audio->Play("Hit_Monster");
		HPChange();
	}
}

void SkellBoss::HPChange()
{
	skellBossUI_->UpdateLifeBar(actorData_.HP, actorData_.maxHP);
	//	playerUI->playerLifeUI_->UpdateLifeBar(actorData_.HP, actorData_.maxHP);
	if (actorData_.HP <= 0)
		Die();
}

void SkellBoss::Die()
{
	actorData_.living = ActorState::DIE;
	EnterDYING();	// ���� ���� ���� ȣ��
	Audio->Stop("JailBoss");	// �����: ��� ���
	Audio->Play("bossDefeat");
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
	_animation->SetPosition(GetPosition());
	_animation->SetRotation(GetRotation());

	pCollider_->SetPosition(GetPosition());
	pCollider_->SetRotation(GetRotation());
	pCollider_->SetScale(0.0f,0.0f);

	Vector2 handPos = GetPosition();
	hand_[0]->SetPosition(handPos.x - handGap.x, handPos.y - handGap.y);	// �޼�
	hand_[1]->SetPosition(handPos.x + handGap.x, handPos.y + handGap.y);	// ������

	back_->SetPosition(GetPosition());
	back_->SetScale(GetScale());
	back_->ModifyPosition(8.0f * GetScale().x, -24.0f * GetScale().y);
}
void SkellBoss::ActionHIDE(Matrix V, Matrix P)
{
	pCollider_->Update(V, P);	// ���������� �浹 �Ұ����ϰ�
}
//END HIDE
/// <summary>
/// APPEAR ���� ////////////////////////////////////////////
/// </summary>
void SkellBoss::SwitchStateAPPEAR()
{
	// alpha���� Ȱ��ȭ�� �� �Ǹ�
	if ((1.0f - appearAlpha) < 0.001f) {
	//	MessageBoxW(nullptr, L"WAIT ����", L"SkellBoss", MB_OK);
		stateEnum_ = eSkellBossState::WAIT;
		SwitchState = std::bind(&SkellBoss::SwitchStateWAIT, this);
		Action = std::bind(&SkellBoss::ActionWAIT, this, std::placeholders::_1, std::placeholders::_2);
		Enter = std::bind(&SkellBoss::EnterWAIT, this);
		_animation->GetTexture()->UpdateColorBuffer(0);
		hand_[0]->GetTexture()->UpdateColorBuffer(0);
		hand_[1]->GetTexture()->UpdateColorBuffer(0);
		CAMERA->SetObject(chaseTarget_);
		skellBossUI_->Update();
		Enter();
	}
}


void SkellBoss::EnterAPPEAR()
{
	_animation->SetPlay(0);	// IDLE
	pCollider_->SetScale(_animation->GetTextureRealSize());
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

	if (waitCycle_ >= selectPatternWaitTime_) {
		// 3���� ���ݻ��� �� �ϳ��� ����
		waitCycle_ = 0.0f;
		
		std::random_device rd;
		std::default_random_engine eng(rd());
		std::uniform_int_distribution<> distr(0, 2);		
		if (beforePatternNum_ == distr(eng)) {
			std::default_random_engine eng(rd());
			beforePatternNum_ = distr(eng);	//�ѹ��� �� ������: �̹����� �����Ŷ߸� ��¿������
		}
		
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
//	hand_[0]->SetPosition(handPos.x - handGap.x, handPos.y - handGap.y);	// �޼�
//	hand_[1]->SetPosition(handPos.x + handGap.x, handPos.y + handGap.y);	// ������
	// Laser �߰��� Wait ������ �ʱ�ȭ �� �ʿ� ��������.
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
	skelMousePos_ = back_->GetPosition();	// �Ѿ˻��� �����Ҷ� skelMousePos ����
	if (bulletDirection_ > 0.0f)
		bulletDirection_ = -1.0f;
	else
		bulletDirection_ = 1.0f;
}

void SkellBoss::ActionBULLET(Matrix V, Matrix P)
{
	// TIMEMANAGER->GetFrame()
	// 144������ ���� 14������, 60������ ���� 6 �����Ӹ���
	if (UpdateBulletCycle(9) && (numOfActiveBullets_ / 4) < (_countof(bullets_) / 4)) {
		// ��� �и�
		bulletRadian_ += PI * 0.04f * bulletDirection_;
		bullets_[numOfActiveBullets_]->Reset();
		bullets_[numOfActiveBullets_++]->Fire(bulletRadian_, skelMousePos_, chaseTarget_);	// reset�� ��ü�ϴ� ���
		bullets_[numOfActiveBullets_]->Reset();
		bullets_[numOfActiveBullets_++]->Fire(bulletRadian_ + (PI * 0.5f), skelMousePos_, chaseTarget_);	// reset�� ��ü�ϴ� ���
		bullets_[numOfActiveBullets_]->Reset();
		bullets_[numOfActiveBullets_++]->Fire(bulletRadian_ + PI, skelMousePos_, chaseTarget_);	// reset�� ��ü�ϴ� ���
		bullets_[numOfActiveBullets_]->Reset();
		bullets_[numOfActiveBullets_++]->Fire(bulletRadian_ + (PI * 1.5f), skelMousePos_, chaseTarget_);	// reset�� ��ü�ϴ� ���

		Audio->Play("BBullet");
	}

	// bullet ������Ʈ ���� �߰� (����: �׷��� Wait ���� active�� ����
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
	// Į�� ���������� Event�� ȣ���ϰ�,0�� �Ǹ� Wait�� �ٲ�� ��.
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
	numOfInactiveSwords_ = 6;	// Į�� ���������� Event�� ȣ���ϰ�,0�� �Ǹ� Wait�� �ٲ�� ��.
}

void SkellBoss::ActionSWORD(Matrix V, Matrix P)
{
	// ���� ���� �ñ⸦ Bullet�� ������.
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
	// ���� �� ����
	std::uniform_int_distribution<> distr2(0, 1);
	laserDirection_ = distr2(eng);	// 0�� ���ʹ߻� ����, 1�� ������ �߻� ����
	// ������ �����̸� �������� �������� "����"���� �߻��ؾ� ��
	endLaser = false;
}

void SkellBoss::ActionLASER(Matrix V, Matrix P)
{
	// �� �ֱ� ����
	if (UpdateBulletCycle(1) && FireLaserCount_ > 0) {
		if (laserDirection_ == 0)
			laserDirection_ = 1;
		else
			laserDirection_ = 0;

		hand_[laserDirection_]->SetPosition(hand_[laserDirection_]->GetPosition().x, chaseTarget_->GetPosition().y);
		// ������ �Ϸ�Ǹ�, �߻�ִϸ��̼� ���
		hand_[laserDirection_]->SetPlay(1, true);
		--FireLaserCount_;
	//	Audio->Play("BossLaser");		// ���� ȣ�� ������ BossLaser SummonLaser�� �ű�. �������� ã���� �ٽ� �ű���� ����.
	}	
	//	Attack�� Hand�� 1�� Ŭ�� �� 8�� Ŭ���� ��
	if (hand_[laserDirection_]->GetAnimationClip(1)->GetCurrentFrame() == 9 && beforeShootLaser == false) {
		// �ִϸ��̼� ��ȣ�� ���� ������ ��ȯ
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

void SkellBoss::SwitchStateDYING()
{
	if (explosionNum_ <= 0) {
		stateEnum_ = eSkellBossState::DEAD;
		SwitchState = std::bind(&SkellBoss::SwitchStateDEAD, this);
		Action = std::bind(&SkellBoss::ActionDEAD, this, std::placeholders::_1, std::placeholders::_2);
		Enter = std::bind(&SkellBoss::EnterDEAD, this);
		Enter();
	}
}
//	 DYING�� ���� ���Ͽ��� �����̴°� �ƴ�, �ܺο��� ȣ���ϴ� ���: �׷��� SwitchState�� �Ϻ� ����� �Ű���.
void SkellBoss::EnterDYING()
{
	CAMERA->SetObject(this);		
	stateEnum_ = eSkellBossState::DYING;
	SwitchState = std::bind(&SkellBoss::SwitchStateDYING, this);
	Action = std::bind(&SkellBoss::ActionDYING, this, std::placeholders::_1, std::placeholders::_2);
	Enter = std::bind(&SkellBoss::EnterDYING, this);
	isPlatform_ = false;
	isGround_ = false;
	isConflicted_ = false;
	dyingTime_ = 0.0f;
	_animation->SetPlay(2);
	back_->SetStop();
	hand_[0]->SetStop();
	hand_[1]->SetStop();
	firstDyingDelay_ = 3.0f;
	explosionNum_ = 10;

	for (UINT i = 0; i < _countof(dieEffect_); ++i) {
		dieEffect_[i] = objectPool->GetMonsterDieEffect();
		(*dieEffect_[i])->SetPlay(0);
		(*dieEffect_[i])->GetAnimationClip(0)->SetTime(0.06f);	// DEAD ���� �ð��� �ٲٱ⿡, �����·� ���ͽ�Ŵ �ϴ�
		(*dieEffect_[i])->SetPosition(GetPosition());
		(*dieEffect_[i])->SetScale(GetScale());
	}
}

void SkellBoss::ActionDYING(Matrix V, Matrix P)
{
	firstDyingDelay_ -= TIMEMANAGER->Delta();
	if (firstDyingDelay_ < 0.0f){
	dyingTime_ += TIMEMANAGER->Delta();
		if (dyingTime_ >= 0.4f && explosionNum_ > 0) {
			dyingTime_ = 0.2f;
			--explosionNum_;
			for (UINT i = 0; i < _countof(dieEffect_); ++i) {
				// ������ ��� ��.
				Vector2 range = _animation->GetTextureRealSize();
	
				std::random_device rd;
				std::default_random_engine eng(rd());
				std::uniform_real_distribution<> distr((double)-range.x * 0.5, (double)range.x * 0.5);
	
				std::uniform_real_distribution<> distr2((double)-range.y * 0.5, (double)range.y * 0.5);
				range = Vector2(distr(eng), distr2(eng));
				(*dieEffect_[i])->SetPosition(GetPosition() + range);
				(*dieEffect_[i])->SetPlay(0, true);
			}
			Audio->Play("MonsterDie");	// �Ĺٹٹٹ� �ؾ���.
		}
	}
	hand_[0]->Update(V, P);
	hand_[1]->Update(V, P);
	back_->Update(V, P);
	_animation->Update(V, P);
	pCollider_->Update(V, P);
	if (dyingTime_ >= 0.2f) {
		for (UINT i = 0; i < _countof(dieEffect_); ++i)
			(*dieEffect_[i])->Update(V, P);
	}
}

void SkellBoss::SwitchStateDEAD()
{	// die x time
	if (dyingTime_ >= 1.0f) {
		CAMERA->SetObject(chaseTarget_);
		eventHandler->Push(L"MonsterDie");
		for (UINT i = 0; i < _countof(dieEffect_); ++i)	{
			if(dieEffect_[i] != nullptr)
				(*dieEffect_[i])->GetAnimationClip(0)->SetTime(0.06f);	// nullptr�� �ٲٱ� ���� ����ȭ
			dieEffect_[i] = nullptr;
		}
	}
	if (dyingTime_ >= 5.0f) {
		SetActive(false);
	}
}

void SkellBoss::ActionDEAD(Matrix V, Matrix P)
{
	dyingTime_ += TIMEMANAGER->Delta();
	if (dyingTime_ >= 0.1f && explosionNum_ > 0) {
		dyingTime_ = 0.0f;
		--explosionNum_;
		for (UINT i = 0; i < _countof(dieEffect_); ++i) {
			// ������ ��� ��.
			Vector2 range;
			Vector2 expolsize = (*dieEffect_[i])->GetTextureRealSize() * 0.1f;
				// 1.0472 = 60���� ����
			range = Vector2(cosf(i * 1.0472f) * expolsize.x, sinf(i * 1.0472f) * expolsize.y);

			// Modify �� Enter ���� �������� �� ��ġ�� �ʱ�ȭ�� ��������.
			(*dieEffect_[i])->ModifyPosition(range * (maxNum_ - explosionNum_));
			(*dieEffect_[i])->SetPlay(0, true);
			(*dieEffect_[i])->GetAnimationClip(0)->SetTime(0.02f);	// ���� 0.06��, �Ҹ��� �� ���º��濡�� ���������.
		}
		Audio->Play("MonsterDie");	// �Ĺٹٹٹ� �ؾ���.
	}
	moveAmount.x = 0.0f;
	GravityUpdate();
	__super::GroundCheck();
	Move();

	if (dieEffect_[0] != nullptr) {
		for (UINT i = 0; i < _countof(dieEffect_); ++i)
			(*dieEffect_[i])->Update(V, P);
	}

	_animation->SetPosition(GetPosition());
	pCollider_->SetPosition(GetPosition());
	pCollider_->Update(V, P);
	_animation->Update(V, P);
}

void SkellBoss::EnterDEAD()
{
	CAMERA->Update();
	CAMERA->SetObject(nullptr);
	for (UINT i = 0; i < _countof(dieEffect_); ++i) {
		(*dieEffect_[i])->SetPosition(GetPosition());	// �������� ���� ��ġ�� ������.
	}
	dyingTime_ = 0.0f; 
	maxNum_ = explosionNum_ = 6;
	_animation->SetPlay(3);
}
