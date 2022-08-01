#include "ImitationGame/framework.h"
#include "./InputHandler/InputHandler.h"
#include "./InputHandler/PlayerInputHandler.h"
#include "./Command/Command.h"
#include "./Physics/Collider.h"
#include "./Object/Inventory.h"
#include "./Object/Item.h"
#include "./Object/Weapons/Weapon.h"
#include "./Object/Line.h"
#include "Player.h"

Player::Player(int AnimationID)
{
	wstring shader = SHADER_FOLDER;	shader += L"TextureColor.hlsl";
	wstring strImage = IMAGE_FOLDER;	strImage += L"char/Adventurer/Idle/CharIdle0.png";
	_animation = new Animation(strImage, shader);
	Texture* pTexture = _animation->GetTexture();
	pTexture->UpdateColorBuffer(0);

	// 0725: Dash Effect �߰�
	for (int i = 0; i < _countof(dashEffect_); ++i) {
		dashEffect_[i] = new Texture(strImage, shader);
		dashEffect_[i]->UpdateColorBuffer(Color(1.0f, 1.0f, 1.0f, 0.8f), 4);
	}

	strImage = IMAGE_FOLDER;	strImage += L"char/Adventurer/CharHand.png";
	hand_[0] = new Texture(strImage, shader);
	hand_[1] = new Texture(strImage, shader);

	// IDLE
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);

		for (int i = 0; i <= 4 ; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"char/Adventurer/Idle/CharIdle" + to_wstring(i) + L".png";
			pClip->AddFrame(pTexture, strImage, 0, 0, 17, 21, 0.1f);
			
		}
		_animation->AddClip(pClip);
	}
	// RUN
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);	
		for (int i = 0; i <= 7; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"char/Adventurer/Run/CharRun" + to_wstring(i) + L".png";
			pClip->AddFrame(pTexture, strImage, 0, 0, 21, 0.075f);	// y �� ���� ( IDLE �ִϸ��̼��� 21�� ���� )

		}
		_animation->AddClip(pClip);
	}
	// JUMP
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);
		for (int i = 0; i <= 0; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"char/Adventurer/Jump/CharJump" + to_wstring(i) + L".png";
			pClip->AddFrame(pTexture, strImage, 0, 0, 0.1f);

		}
		_animation->AddClip(pClip);
	}
	SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
	pCollider_ = new Collider();
	Inventory_ = new Inventory();
	inputHandler_ = (InputHandler*) new PlayerInputHandler();
	
	UpdateHandedWeapon();
	Inventory_->SetInventoryOwner(this);	// �ش� �κ��丮�� �����ڸ� Player�� ��.

	// Hand ��ġ ����
	for (auto hand : hand_) {
		hand->SetScale(this->GetScale());
	}

	// actor ������ �޾ƿ���
	actorData_.ImmuneTime = 1;
	actorData_.HP = 20;
	actorData_.maxHP = 20;
	actorData_.type = ActorType::Player;
	// �÷��̾� ���� ���� �߰�

	// �ݹ� ����
	dashCB = std::bind(&Player::DashWaiting, this);

}

Player::~Player()
{
	SAFE_DELETE(_animation);
	SAFE_DELETE(inputHandler_);
	SAFE_DELETE(Inventory_);
	SAFE_DELETE(pCollider_);
}


void Player::Update(Matrix V, Matrix P)
{	
	RotateToMouse();
	InputUpdate();
	dashCB();
	GroundCheck();
	GravityUpdate();
	Move(moveAmount);	// ���������� ������ �̵����� �ݿ�
	CycleUpdate();

	_animation->SetPlay(_currentState + _heroType);
	_animation->SetRotation(GetRotation());
	_animation->SetPosition(GetPosition());
	_animation->SetScale(GetScale());	 
	_animation->Update(V, P);
	dashEffect_[0]->Update(V, P);	// �׽�Ʈ�ڵ�
	dashEffect_[1]->Update(V, P);	// �׽�Ʈ�ڵ�
	dashEffect_[2]->Update(V, P);	// �׽�Ʈ�ڵ�
		
	pCollider_->SetScale(_animation->GetTexture()->GetTextureRealSize());
	pCollider_->SetRotation(GetRotation());
	pCollider_->SetPosition(GetPosition());
	pCollider_->Update(V, P);
	CAMERA->Update(V, P);
	Inventory_->Update(P);	// UI �迭�� ������ǥ�� �ʿ��ϴ�.

	if (handedWeapon_[currentFocusHand_ * 2] != nullptr) {
		// ���콺 ���� ������ ��� weapon�� ��ӹ޾� ������ ���� �ν��Ͻ����� ����
		// ����� ��ü���� ������ ������ �������� �ʱ�ȭ��.
		handedWeapon_[currentFocusHand_ * 2]->SetWeaponPosition(_position);
//		handedWeapon_[currentFocusHand_ * 2]->SetWeaponScale(_scale);
		handedWeapon_[currentFocusHand_ * 2]->SetWeaponRotation(_rotation);
		handedWeapon_[currentFocusHand_ * 2]->Update(V, P);
	}
	if (handedWeapon_[(currentFocusHand_ * 2) + 1 ] != nullptr) {
		handedWeapon_[(currentFocusHand_ * 2) + 1]->SetWeaponPosition(_position);
//		handedWeapon_[(currentFocusHand_ * 2) + 1]->SetWeaponScale(_scale);
		handedWeapon_[(currentFocusHand_ * 2) + 1]->SetWeaponRotation(_rotation);
		handedWeapon_[(currentFocusHand_ * 2) + 1]->Update(V, P);
	}

	// �� ������Ʈ �������� �����ؼ� �и�
	HandUpdate(V, P);

	ImmuneFrame_ = max(0, ImmuneFrame_ - 1);
}

void Player::RotateToMouse()
{
	Vector2 mousePos = Mouse->GetPosition();
	CAMERA->WCtoVC(mousePos);

	float result = mousePos.x - _position.x;	
	float sign = copysign(1, result);	// copysign ���� ���
	sign -= 1.0f;	// ��� 0, ���� -2

	SetRotation(0.0f, sign * 90.0f, 0.0f);
}

void Player::Render()
{
	dashEffect_[0]->Render();
	dashEffect_[1]->Render();
	dashEffect_[2]->Render();
	_animation->Render();
	pCollider_->Render();
	Inventory_->Render();

	if (handedWeapon_[currentFocusHand_ * 2] != nullptr)
		handedWeapon_[currentFocusHand_ * 2]->Render();
	if (handedWeapon_[currentFocusHand_ * 2 + 1] != nullptr)
		handedWeapon_[currentFocusHand_ * 2 + 1]->Render();
		// �տ� �� ���⸸ Update �� Render
	
	// ���� ���� ���Ŀ� Render
	hand_[0]->Render();
	hand_[1]->Render();
}

void Player::Reset()
{
	_currentState = State::IDLE;
	maxImmuneFrame_ = actorData_.ImmuneTime * TIMEMANAGER->GetFrame();

	// �÷��̾� �κ��丮�ϱ� �÷��̾ ����
	playerUI->Reset();
	playerUI->playerLifeUI_->UpdateLifeBar(actorData_.HP, actorData_.maxHP);
}
// ĳ���͸� �ٲٷ��� �� �� �����
void Player::ChangeChar(objectType playerType)
{
}
// GameActor�� �ű�, �׷��� ��� ���������� �ț��⿡ ��������.
void Player::GroundCheck()
{
	Scene* tempScene = SCENEMANAGER->GetCurrentScene();
	Line* m_pGroundLine = tempScene->GetGroundLines();
	Line* m_pCeilingLine = tempScene->GetCeilingLines();
	Line* m_pPlatformLine = tempScene->GetPlatformLines();
	bool flag = false;
	isConflicted_ = false;
	for (UINT i = 0; i < m_pGroundLine->GetCountLine(); i++) {
		Vector2 start = m_pGroundLine->GetStartPoint(i);
		Vector2 end = m_pGroundLine->GetEndPoint(i);
		Vector2 mStart = pCollider_->GetPosition();
		Vector2 mEnd;
		mEnd.x = mStart.x;
		mEnd.y = mStart.y - pCollider_->GetScale().y * 0.5f;
		Vector2 result;
		// �Ʒ��� �� �˻�
		if (!isJump && Line::IntersectionLine(start, end, mStart, mEnd, result))
		{
			Vector2 charPos = GetPosition();
			float fRad = atan2f(end.y - start.y, end.x - start.x);
			float Slope = (end.y - start.y) / (end.x - start.x);
			charPos.y = Slope * (charPos.x - start.x) + start.y + _animation->GetTextureRealSize().y * 0.5f;
			if (dashStart_ == false)
				SetY(charPos.y);
			else
				break;
			flag = true;
			SetGroundCheck(true);
			if(_currentState != State::RUN)
				_currentState = State::IDLE;
			longJumpCount_ = 0.0f;
			isCanlongJump_ = true;
			isLongJump_ = false;
			isJump = false;
			isFall = false;
		}
	}//end for

	// �÷������� �浹�˻� ( ����߿��� �÷��� �浹 �Ұ�: ���߿�)
	if (true || isDash == false) {
		for (int i = 0; i < m_pPlatformLine->GetCountLine(); i++) {
			Vector2 start = m_pPlatformLine->GetStartPoint(i);
			Vector2 end = m_pPlatformLine->GetEndPoint(i);
			Vector2 mStart = pCollider_->GetPosition();
			Vector2 mEnd;
			mEnd.x = mStart.x;
			mEnd.y = mStart.y - pCollider_->GetScale().y * 0.5f;
			Vector2 result;
			// �Ʒ��� �� �˻�
			if (!isJump && Line::IntersectionLine(start, end, mStart, mEnd, result))
			{
				Vector2 charPos = GetPosition();
				float fRad = atan2f(end.y - start.y, end.x - start.x);
				float Slope = (end.y - start.y) / (end.x - start.x);
				charPos.y = Slope * (charPos.x - start.x) + start.y + _animation->GetTextureRealSize().y * 0.5f;
				if (dashStart_ == false)
					SetY(charPos.y);
				else
					break;
				flag = true;
				SetGroundCheck(true);
				isPlatform_ = true;		// �÷��� �߰�(��������� ����� ����)
				if (_currentState != State::RUN)
					_currentState = State::IDLE;
				longJumpCount_ = 0.0f;
				isCanlongJump_ = true;
				isLongJump_ = false;
				isJump = false;
				isFall = false;
				break;
			}
		}//end for
	}
	for (UINT i = 0; i < m_pCeilingLine->GetCountLine(); i++)
	{
		Vector2 start = m_pCeilingLine->GetStartPoint(i);
		Vector2 end = m_pCeilingLine->GetEndPoint(i);

		Vector2 charPos = GetPosition();
		Vector2 size = pCollider_->GetScale();
		Vector2 left = Vector2(charPos.x - size.x * 0.5f, charPos.y);
		Vector2 right = Vector2(charPos.x + size.x * 0.5f, charPos.y);
		Vector2 top = Vector2(charPos.x, charPos.y + (size.y * 0.5f));
		Vector2 result;

		if (isConflicted_ = Collider::InterSectionLine(charPos, right, start, end)) {
			moveAmount.x = -0.5f * WSCALEX;
			break;
		}
		if (isConflicted_ = Collider::InterSectionLine(charPos, left, start, end)) {
			moveAmount.x = +0.5f * WSCALEX;
			break;
		}
		if (isConflicted_ = Collider::InterSectionLine(charPos, top, start, end)) {
			moveAmount.y = -0.5f * WSCALEY;
			break;
		}
	}
	isGround_ = flag;
	dashStart_ = false;	// �׻� ���˻� ���Ŀ� false��
}


void Player::CollisionCheck()
{
}

void Player::InputUpdate()
{
	vector<Command*> commandQueue = inputHandler_->handleInput();	// ��¥�� ť�� �ƴϾ�
	for (Command* command : commandQueue) {
 		GameActor* tempActor = this;
		command->execute(*tempActor);
	}
}

void Player::HandUpdate(Matrix V, Matrix P)
{
	Vector3 thisRotation = _animation->GetRotation();
	// ���� �ȵ� ������
	float vec = std::abs(thisRotation.y / 180.0f);	// ���밪�� �־�� 180 , -180�� ���� �����ο���.
	vec *= 2.0f;
	vec -= 1.0f;	// ���� -1 ���� 1 ���� ������
	float gap = 35.0f;
	hand_[1]->SetPosition(this->GetPosition().x + (vec * gap * WSCALEX), this->GetPosition().y - gap * WSCALEY);
	hand_[1]->Update(V, P);

	// ���� �� �޼�
	Vector3 afterRotation;
	Vector2 beforePosition;

	if (handedWeapon_[currentFocusHand_ * 2] != nullptr) {
		hand_[0]->SetPosition(handedWeapon_[currentFocusHand_ * 2]->GetLeftHandPoint());
		hand_[0]->SetRotation(handedWeapon_[currentFocusHand_ * 2]->GetLeftHandRotation());
		hand_[0]->_pivot = handedWeapon_[currentFocusHand_ * 2]->GetLeftHandPivot();
	}
	else	// �� �⺻��ġ
	{
		hand_[0]->SetPosition(this->GetPosition().x - (vec * gap * 1.2f * WSCALEX), this->GetPosition().y - 20.0f * WSCALEY);
		float angle = Mouse->GetAngleRelativeToMouse(hand_[0]->GetPosition().x, hand_[0]->GetPosition().y);
		hand_[0]->SetRotation(0.0f, 0.0f, angle);
		hand_[0]->_pivot = Vector3(0.0f, 0.0f, 0.0f);
	}
	hand_[0]->Update(V, P);
}
// ���� ����Ŭ�� ���� ������Ʈ
void Player::CycleUpdate()
{
	dashCycleTime_ += TIMEMANAGER->Delta();
	if (dashCycleTime_ >= playerData_.dashRechargeTime) {
		dashCycleTime_ = 0.0f;
		DashRecharge(1);
	}
}
// GameActor ����Լ��� ����
/*
void Player::GravityUpdate()
*/
void Player::UpdateHandedWeapon()
{
	// ������ ������ ǥ�����ֱ�
	for (int i = 0; i < _countof(handedWeapon_); ++i)
		handedWeapon_[i] = nullptr;

	for (int i = 0; i < _countof(handedWeapon_); ++i) {
		handedWeapon_[i] = (Weapon*)Inventory_->GetEquipItem(i);
		if (handedWeapon_[i] != nullptr) {
			handedWeapon_[i]->SetOwner((GameActor*)this);	// ���� ������ ������ �۵����� �Ϲ� �����ͷ� ����
		}
	}
}

void Player::Attacked(float damage)
{
//	eventHandler->Push(L"playerAttacked");	// ���� �����������Ҷ� �ʿ��ϸ� ����. �� �̺�Ʈ�� �ƴҼ��� �ִ�.
	float changed = max(damage - actorData_.armor, 1.0f);
	DecreaseHP((int)changed);
	Audio->Play("Hit_Player");
	ImmuneFrame_ = maxImmuneFrame_;
	//Explosion.wav ������ �̰� ȣ���ϸ� ��.
}

void Player::Dash()
{
	if (playerData_.dashCount <= 0)
		return;

	Audio->Play("Dash_Player");
	// player�� Dash �Ϸ��� ���� 
	playerData_.dashCount--;	// 1. ī��Ʈ ����
	dashRadian = Mouse->GetAngleRelativeToMouse(this->_position.x, this->_position.y, 1);	// ���� ���ϱ�

	moveAmount = Vector2(cosf(dashRadian) * playerData_.baseSpeed * 2.0f * TIMEMANAGER->Delta() * WSCALEX,
		sinf(dashRadian) * playerData_.baseSpeed * 2.0f * TIMEMANAGER->Delta() * WSCALEY);
	dashStart_ = true;
	if (dashRadian >= 0.0f)
		moveAmount.y += 5.0f * WSCALEY;
	dashLifeCycle = 0.5f;	// ����������Ŭ �ʱ�ȭ.
	dashCB = std::bind(&Player::DashDo, this);	// �� �ݹ��� Update �ʿ��� �����.
	// ���� UI ����
	playerUI->dashUI_->Dash();

}
// Dash �� ���� �ִϸ��̼��� �����ϸ� lifeCycle ���� 
void Player::DashDo()
{
	isDash = true;
	// �̺κ��� Dash() ���� �� �ѹ� ���شٴ� �������� �ٲ�
//	this->ModifyPosition(cosf(dashRadian) * playerData_.baseSpeed* 1.5f * TIMEMANAGER->Delta() * WSCALEX,
//		sinf(dashRadian) * playerData_.baseSpeed * 1.5f * TIMEMANAGER->Delta() * WSCALEY);
	dashLifeCycle -= TIMEMANAGER->Delta();
	DashAnimationUpdate();

	if (dashLifeCycle <= 0.0f) {
		dashLifeCycle = 0.5f;	// Ȥ�� �𸣴� �ʱ�ȭ
		dashCB = std::bind(&Player::DashWaiting, this);
		isDash = false;

		dashEffect_[0]->SetScale(0.0f, 0.0f);
		dashEffect_[1]->SetScale(0.0f, 0.0f);
		dashEffect_[2]->SetScale(0.0f, 0.0f);
	}
}

void Player::DashWaiting()
{
	// ��� �ƹ��͵� ����. 
}

// 0 1 2 3  01�� 1��, 23�� 2��
void Player::SetHandedWeapon(Weapon * item, int index)
{
	int newIndex = min(index, (int)_countof(handedWeapon_) - 1);
	handedWeapon_[newIndex] = item;
}
/*
auto Player::GetHandedWeapon(int index)
{
	int newIndex = min(index, ((int)sizeof(handedWeapon_) / (int)sizeof(handedWeapon_[0])) - 1);
	return handedWeapon_[newIndex];
}
*/
// 0 1 2 3  01�� 1��, 23�� 2��
Weapon* Player::GetHandedWeapon(int index)
{
	int newIndex = min(index, ((int)sizeof(handedWeapon_) / (int)sizeof(handedWeapon_[0])) - 1);
	return handedWeapon_[newIndex];
}

void Player::IncreaseHP(int amount)
{
	actorData_.HP = min(actorData_.maxHP, actorData_.HP + amount);
	HPChange();
}

void Player::DecreaseHP(int amount)
{
	actorData_.HP = max(0, actorData_.HP - amount);
	HPChange();
}

void Player::HPChange()
{
	playerUI->playerLifeUI_->UpdateLifeBar(actorData_.HP, actorData_.maxHP);
	if (actorData_.HP <= 0)
		FatalBlow();	// �̰� actor ����
}

void Player::Die()
{
	printf("PlayerDie\n");
}

// ������ x ���� moveSpeed x speed xy ��ŭ �̵��ϴ� �Լ�
void Player::LeftMove()
{
	if(isDash == false)
		moveAmount.x = -playerData_.baseSpeed * TIMEMANAGER->Delta() * WSCALEX;
	if (_currentState != State::JUMP)
		_currentState = State::RUN;

}
//	moveAmout�� �����Ű�� ���
void Player::RightMove()
{
	if (isDash == false)
		moveAmount.x = playerData_.baseSpeed * TIMEMANAGER->Delta() * WSCALEX;
	if(_currentState != State::JUMP)
		_currentState = State::RUN;
}
// ���߿� �Ẹ��.
void Player::Move(Vector2& position)
{
	// Collider ���� �浹 �����ϱ�
	ModifyPosition(position);
}

void Player::DashRecharge(int amount)
{
	playerData_.dashCount = min(playerData_.maxDashCount, playerData_.dashCount + amount);	
	playerUI->dashUI_->IncreaseDashCount(amount);
}
// Next: ������ ������� �ִϸ��̼� ����, ���� �ȴٸ� 01, 12 20 ������ index�� ��ȸ�ϸ� �����ֱ⸦ ������ ��
void Player::DashAnimationUpdate()
{
	if (dashLifeCycle <= 0.1f) {
		dashEffect_[2]->SetScale(0.0f, 0.0f);
	}
	else if (dashLifeCycle <= 0.15f) {
		dashEffect_[1]->SetScale(0.0f, 0.0f);
	}
	else if (dashLifeCycle <= 0.2f) {
		dashEffect_[0]->SetScale(0.0f, 0.0f);
	}
	else if (dashLifeCycle <= 0.25f) {
		dashEffect_[2]->SetPosition(GetPosition());
		dashEffect_[2]->SetRotation(GetRotation());
		dashEffect_[2]->SetScale(GetScale());
	}
	else if (dashLifeCycle <= 0.3f) {
		dashEffect_[1]->SetPosition(GetPosition());
		dashEffect_[1]->SetRotation(GetRotation());
		dashEffect_[1]->SetScale(GetScale());
	}
	else if (dashLifeCycle <= 0.35f) {
		dashEffect_[0]->SetPosition(GetPosition());
		dashEffect_[0]->SetRotation(GetRotation());
		dashEffect_[0]->SetScale(GetScale());
	}
	else if (dashLifeCycle <= 0.4f) {
		dashEffect_[2]->SetPosition(GetPosition());
		dashEffect_[2]->SetRotation(GetRotation());
		dashEffect_[2]->SetScale(GetScale());
	}
	else if (dashLifeCycle <= 0.45f) {
		dashEffect_[1]->SetPosition(GetPosition());
		dashEffect_[1]->SetRotation(GetRotation());
		dashEffect_[1]->SetScale(GetScale());
	}
	else if (dashLifeCycle <= 0.5f) {
		dashEffect_[0]->SetPosition(GetPosition());
		dashEffect_[0]->SetRotation(GetRotation());
		dashEffect_[0]->SetScale(GetScale());
	}

}

void Player::Jump()
{
	// ���� ����
	if (isGround_ == true) {
		isJump = true;
		isGround_ = false;
		_currentState = State::JUMP;
		moveAmount.y = playerData_.baseJumpSpeed * 0.05f * WSCALEY;
		longJumpCount_ = 0.0f;
	}
	else if (isLongJump_ == false && isCanlongJump_ == true) {
		longJumpCount_ += TIMEMANAGER->Delta();
		if (longJumpCount_ > 0.16f) {
			moveAmount.y += playerData_.baseLongJumpSpeed * 0.05f * WSCALEY;
			isLongJump_ = true;
			isCanlongJump_ = false;
		}
	}
}

void Player::Idle()
{
	if(isDash == false)
		moveAmount.x = 0.0f;
	if(_currentState != State::JUMP)
	_currentState = State::IDLE;
}

void Player::Attack()
{
	// �ִϸ��̼�
	if (handedWeapon_[currentFocusHand_ * 2] != nullptr){
		handedWeapon_[currentFocusHand_ * 2]->Fire();
	}
	if (handedWeapon_[currentFocusHand_ * 2 + 1] != nullptr) {
		handedWeapon_[currentFocusHand_ * 2 + 1]->Fire();
	}
}

void Player::InventoryToggle()
{
	if(Inventory_->IsActive())
		Inventory_->SetActive(false);
	else{
		Inventory_->SetActive(true);
	}
}

void Player::SwapHandFocus()
{
	if (currentFocusHand_ == 0)
		currentFocusHand_ = 1;
	else if (currentFocusHand_ == 1)
		currentFocusHand_ = 0;
	else
		MessageBoxW(MAIN->GetWindowHandler(), L"handFocus�� 0�� 1�ۿ� �����ϴ�.", L"Player::SwapHandFocus()", MB_OK);
	Inventory_->SetFocusHand(currentFocusHand_);
	Inventory_->SetFocusPosition();
	eventHandler->Push(L"SwapItem");
	UpdateHandedWeapon();
}
