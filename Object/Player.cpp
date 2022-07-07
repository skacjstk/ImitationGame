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
			pClip->AddFrame(pTexture, strImage, 0, 0, 0.075f);	// 끝모양이 다 달라서 이렇게 했음.

		}
		_animation->AddClip(pClip);
	}
	// JUMP
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);
		for (int i = 0; i <= 0; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"char/Adventurer/Jump/CharJump" + to_wstring(i) + L".png";
			pClip->AddFrame(pTexture, strImage, 0, 0, 0.1f);	// 끝모양이 다 달라서 이렇게 했음.

		}
		_animation->AddClip(pClip);
	}
	SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
	pCollider_ = new Collider();
	Inventory_ = new Inventory();
	tempLine_ = new Line();

	tempLine_->AddLine(-500.0f, -200.0f, 500.0f, -200.0f);
	tempLine_->AddLine(500.0f, -200.0f, 700.0f, -300.0f);
	tempLine_->EndLine();
	inputHandler_ = (InputHandler*) new PlayerInputHandler();
	
	UpdateHandedWeapon();
	Inventory_->SetInventoryOwner(this);	// 해당 인벤토리의 소유자를 Player로 함.
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
	Inventory_->Update(P);	// UI 계열은 절대좌표가 필요하다.

	if (handedWeapon_[currentFocusHand_ * 2] != nullptr) {
		// 마우스 각도 관련은 모두 weapon을 상속받아 구현한 무기 인스턴스에서 관리
		// 무기는 자체적인 배율을 생성자 시점에서 초기화함.

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

	tempLine_->Update(V, P);
}

void Player::Render()
{
	_animation->Render();
	pCollider_->Render();
	Inventory_->Render();
	tempLine_->Render();

	for (int i = 0; i < _countof(handedWeapon_); ++i)
	{
		if (handedWeapon_[currentFocusHand_ * 2] != nullptr)
			handedWeapon_[currentFocusHand_ * 2]->Render();
		if (handedWeapon_[currentFocusHand_ * 2 + 1] != nullptr)
			handedWeapon_[currentFocusHand_ * 2 + 1]->Render();
	}	// 손에 든 무기만 Update 및 Render
}

void Player::Reset()
{
	_currentState = State::IDLE;
}
// 캐릭터를 바꾸려고 할 때 사용할
void Player::Reset(objectType playerType)
{
}

void Player::GroundCheck()
{
	bool flag = false;
	// 테스트코드: 어쩌면 얘도 Collider를 Line으로 바꿔야 할지도 모름, 필요한 것: 대각선 Line 따라 걸어가는 거 Next
	for (int i = 0; i < tempLine_->GetCountLine(); ++i) {
		Vector2 start = tempLine_->GetStartPoint(i);
		Vector2 end = tempLine_->GetEndPoint(i);
		if (Line::IntersectionLine(start, end, this->GetCollider()))
		{
	//		isGround_ = true;
			flag = true;
			_currentState = State::IDLE;
			longJumpCount_ = 0.0f;
			isCanlongJump_ = true;
			isLongJump_ = false;
			break;
		}			
	}
	isGround_ = flag;
}

void Player::InputUpdate()
{
	vector<Command*> commandQueue = inputHandler_->handleInput();	// 진짜로 큐는 아니야
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
	// 무기의 소유자 표시해주기
	for (int i = 0; i < _countof(handedWeapon_); ++i)
		handedWeapon_[i] = nullptr;

	for (int i = 0; i < _countof(handedWeapon_); ++i) {
		handedWeapon_[i] = (Weapon*)Inventory_->GetEquipItem(i);
		if (handedWeapon_[i] != nullptr) {
			handedWeapon_[i]->SetOwner((GameActor**)this);	// 될지모르겠다
		}
	}
}

// 0 1 2 3  01은 1번, 23은 2번
void Player::SetHandedWeapon(Weapon * item, int index)
{
	int newIndex = min(index, (int)_countof(handedWeapon_) - 1);
	handedWeapon_[newIndex] = item;
}
// 0 1 2 3  01은 1번, 23은 2번
auto Player::GetHandedWeapon(int index)
{
	int newIndex = min(index, ((int)sizeof(handedWeapon_) / (int)sizeof(handedWeapon_[0])) - 1);
	return handedWeapon_[newIndex];
}

// 왼쪽이 x 음수 moveSpeed x speed xy 만큼 이동하는 함수
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
// 해당 위치로 움직이려고 시도하며 위치가 바뀜( 이 위치는 중력 등 기타 무언가에 의해 바뀔 수 있음)
void Player::Move(Vector2& position)
{
	// Collider 등의 충돌 적용하기
	SetPosition(position);
}

void Player::Jump()
{
	// 점프 수행
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
	// 애니메이션
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
		MessageBoxW(MAIN->GetWindowHandler(), L"handFocus는 0과 1밖에 없습니다.", L"Player::SwapHandFocus()", MB_OK);
	Inventory_->SetFocusHand(currentFocusHand_);
	Inventory_->SetFocusPosition();

	UpdateHandedWeapon();
}
