#include "ImitationGame/framework.h"
#include "./InputHandler/InputHandler.h"
#include "./InputHandler/PlayerInputHandler.h"
#include "./Command/Command.h"
#include "./Physics/Collider.h"
#include "./Object/Inventory.h"
#include "./Object/Item.h"
#include "./Object/Weapons/Weapon.h"
#include "Player.h"

Player::Player(int AnimationID)
{
	wstring shader = SHADER_FOLDER;	shader += L"TextureColor.hlsl";
	wstring strImage = IMAGE_FOLDER;	strImage += L"char/Adventurer/Idle/CharIdle0.png";
	_animation = new Animation(strImage, shader);
	Texture* pTexture = _animation->GetTexture();
	pTexture->UpdateColorBuffer(0);

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
			pClip->AddFrame(pTexture, strImage, 0, 0, 0.075f);	// ������� �� �޶� �̷��� ����.

		}
		_animation->AddClip(pClip);
	}
	// JUMP
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);
		for (int i = 0; i <= 0; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"char/Adventurer/Jump/CharJump" + to_wstring(i) + L".png";
			pClip->AddFrame(pTexture, strImage, 0, 0, 0.1f);	// ������� �� �޶� �̷��� ����.

		}
		_animation->AddClip(pClip);
	}
	SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
	pCollider_ = new Collider();
	Inventory_ = new Inventory();
	inputHandler_ = (InputHandler*) new PlayerInputHandler();
	
	UpdateHandedWeapon();
	Inventory_->SetInventoryOwner(this);	// �ش� �κ��丮�� �����ڸ� Player�� ��.
}

Player::~Player()
{
	SAFE_DELETE(_animation);
	SAFE_DELETE(pCollider_);
}


void Player::Update(Matrix V, Matrix P)
{	
	GroundCheck();
	InputUpdate();
	GravityUpdate();

	_animation->SetPlay(_currentState + _heroType);
	_animation->SetRotation(GetRotation());
	_animation->SetPosition(GetPosition());
	_animation->SetScale(GetScale());	 
	_animation->Update(V, P);

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
}

void Player::Render()
{
	_animation->Render();
	pCollider_->Render();
	Inventory_->Render();

	for (int i = 0; i < _countof(handedWeapon_); ++i)
	{
		if (handedWeapon_[currentFocusHand_ * 2] != nullptr)
			handedWeapon_[currentFocusHand_ * 2]->Render();
		if (handedWeapon_[currentFocusHand_ * 2 + 1] != nullptr)
			handedWeapon_[currentFocusHand_ * 2 + 1]->Render();
	}	// �տ� �� ���⸸ Update �� Render
}

void Player::Reset()
{
	_currentState = State::IDLE;
}
// ĳ���͸� �ٲٷ��� �� �� �����
void Player::Reset(objectType playerType)
{
}

void Player::GroundCheck()
{
	// Test �ڵ�
	if (this->GetPosition().y <= -300.0f) {
		isGround_ = true;
		_currentState = State::IDLE;
		longJumpCount_ = 0.0f;
		isCanlongJump_ = true;
		isLongJump_ = false;
	}
	else {
		isGround_ = false;
	}		
}

void Player::InputUpdate()
{
	vector<Command*> commandQueue = inputHandler_->handleInput();	// ��¥�� ť�� �ƴϾ�
	for (Command* command : commandQueue) {
		GameActor* tempActor = this;
		command->execute(*tempActor);
	}
}

void Player::GravityUpdate()
{
	Vector2 position = GetPosition();
	if (isGround_ == false)
		gravity_ -= G * TIMEMANAGER->Delta() * 1.5f;
	else
		gravity_ = 0.0f;
	position.y += gravity_;
	SetPosition(position);
}

void Player::UpdateHandedWeapon()
{
	// ������ ������ ǥ�����ֱ�
	for (int i = 0; i < _countof(handedWeapon_); ++i)
		handedWeapon_[i] = nullptr;

	for (int i = 0; i < _countof(handedWeapon_); ++i) {
		handedWeapon_[i] = (Weapon*)Inventory_->GetEquipItem(i);
		if (handedWeapon_[i] != nullptr) {
			handedWeapon_[i]->SetOwner((GameActor**)this);	// �����𸣰ڴ�
		}
	}
}

// 0 1 2 3  01�� 1��, 23�� 2��
void Player::SetHandedWeapon(Weapon * item, int index)
{
	int newIndex = min(index, (int)_countof(handedWeapon_) - 1);
	handedWeapon_[newIndex] = item;
}
// 0 1 2 3  01�� 1��, 23�� 2��
auto Player::GetHandedWeapon(int index)
{
	int newIndex = min(index, ((int)sizeof(handedWeapon_) / (int)sizeof(handedWeapon_[0])) - 1);
	return handedWeapon_[newIndex];
}

// ������ x ���� moveSpeed x speed xy ��ŭ �̵��ϴ� �Լ�
void Player::LeftMove()
{
	Vector2 position = this->GetPosition();

	position.x -= playerData_.baseSpeed * TIMEMANAGER->Delta();
	if (_currentState != State::JUMP)
		_currentState = State::RUN;
	SetRotation(0.0f, 180.0f, 0.0f);
	Move(position);
}

void Player::RightMove()
{
	Vector2 position = this->GetPosition();

	position.x += playerData_.baseSpeed * TIMEMANAGER->Delta();
	SetRotation(0.0f, 0.0f, 0.0f);
	if(_currentState != State::JUMP)
		_currentState = State::RUN;
	Move(position);
}
// �ش� ��ġ�� �����̷��� �õ��ϸ� ��ġ�� �ٲ�( �� ��ġ�� �߷� �� ��Ÿ ���𰡿� ���� �ٲ� �� ����)
void Player::Move(Vector2& position)
{
	// Collider ���� �浹 �����ϱ�
	SetPosition(position);
}

void Player::Jump()
{
	// ���� ����
	if (isGround_ == true) {
		_currentState = State::JUMP;
		gravity_ = playerData_.baseJumpSpeed * 0.05f;
		isGround_ = false;
		longJumpCount_ = 0.0f;
	}
	else if (isLongJump_ == false && isCanlongJump_ == true) {
		longJumpCount_ += TIMEMANAGER->Delta();
		if (longJumpCount_ > 0.16f) {
			gravity_ += playerData_.baseLongJumpSpeed * 0.05f;
			isLongJump_ = true;
			isCanlongJump_ = false;
		}
	}
}

void Player::Idle()
{
	if(_currentState != State::JUMP)
	_currentState = State::IDLE;
}

void Player::Attack()
{

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

	UpdateHandedWeapon();
}
