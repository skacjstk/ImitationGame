#include "ImitationGame/framework.h"
#include "Physics/Collider.h"
#include "State/State.h"
#include "Object/Bullet/BossBullet.h"
#include "SkellBoss.h"

void SkellBoss::GenerateBullet()
{
	for (UINT i = 0; i < _countof(bullets_); ++i) {
		bullets_[i] = new BossBullet();	// 18 �� ���� 10�� �Űܰ��鼭
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
	thread t(std::bind(&SkellBoss::GenerateBullet, this));
	t.detach();
	// ������ȭ �ϱ�  240���� �����ɸ�
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
}

void SkellBoss::Reset()
{
	SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
	_animation->SetScale(GetScale());
	pCollider_->SetScale(_animation->GetTextureRealSize());	// Wait, Hide ���� �ѹ��� ��ȭ��
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

	// �Կ��� ������ ������ ����
	back_->SetPosition(GetPosition());
	back_->SetScale(GetScale());
	back_->ModifyPosition(7.0f * GetScale().x, -22.0f * GetScale().y);

	pCollider_->SetPosition(GetPosition());
	pCollider_->SetRotation(GetRotation());
	Enter();	// HIDE�� ������
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

	if (waitCycle_ >= 2.0f) {
		// 3���� ���ݻ��� �� �ϳ��� ����
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
	hand_[0]->SetPosition(handPos.x - handGap.x, handPos.y - handGap.y);	// �޼�
	hand_[1]->SetPosition(handPos.x + handGap.x, handPos.y + handGap.y);	// ������
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
	if (UpdateBulletCycle() && (numOfActiveBullets_ / 4) < (_countof(bullets_) / 4)) {
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
