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

	// 0725: Dash Effect 추가
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
			pClip->AddFrame(pTexture, strImage, 0, 0, 21, 0.075f);	// y 값 고정 ( IDLE 애니메이션인 21에 맞춤 )

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
	Inventory_->SetInventoryOwner(this);	// 해당 인벤토리의 소유자를 Player로 함.

	// Hand 위치 설정
	for (auto hand : hand_) {
		hand->SetScale(this->GetScale());
	}

	// actor 데이터 받아오기
	actorData_.ImmuneTime = 1;
	actorData_.HP = 20;
	actorData_.maxHP = 20;
	actorData_.type = ActorType::Player;
	// 플레이어 공통 사운드 추가

	// 콜백 설정
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
	Move(moveAmount);	// 최종적으로 정해진 이동량을 반영
	CycleUpdate();

	_animation->SetPlay(_currentState + _heroType);
	_animation->SetRotation(GetRotation());
	_animation->SetPosition(GetPosition());
	_animation->SetScale(GetScale());	 
	_animation->Update(V, P);
	dashEffect_[0]->Update(V, P);	// 테스트코드
	dashEffect_[1]->Update(V, P);	// 테스트코드
	dashEffect_[2]->Update(V, P);	// 테스트코드
		
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

	// 손 업데이트 생각보다 복잡해서 분리
	HandUpdate(V, P);

	ImmuneFrame_ = max(0, ImmuneFrame_ - 1);
}

void Player::RotateToMouse()
{
	Vector2 mousePos = Mouse->GetPosition();
	CAMERA->WCtoVC(mousePos);

	float result = mousePos.x - _position.x;	
	float sign = copysign(1, result);	// copysign 최초 사용
	sign -= 1.0f;	// 양수 0, 음수 -2

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
		// 손에 든 무기만 Update 및 Render
	
	// 무기 랜더 이후에 Render
	hand_[0]->Render();
	hand_[1]->Render();
}

void Player::Reset()
{
	_currentState = State::IDLE;
	maxImmuneFrame_ = actorData_.ImmuneTime * TIMEMANAGER->GetFrame();

	// 플레이어 인벤토리니까 플레이어가 리셋
	playerUI->Reset();
	playerUI->playerLifeUI_->UpdateLifeBar(actorData_.HP, actorData_.maxHP);
}
// 캐릭터를 바꾸려고 할 때 사용할
void Player::ChangeChar(objectType playerType)
{
}
// GameActor에 옮김, 그러나 얘는 상태패턴을 안썻기에 재정의함.
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
		// 아래와 선 검사
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
			isPlatform_ = false;
			if(_currentState != State::RUN)
				_currentState = State::IDLE;
			longJumpCount_ = 0.0f;
			isCanlongJump_ = true;
			isLongJump_ = false;
			isJump = false;
			isFall = false;
		}
	}//end for

	// 플랫폼라인 충돌검사 ( 대시중에는 플랫폼 충돌 불가: 나중에)
	if (isUnderJump_ == false) {
		for (int i = 0; i < m_pPlatformLine->GetCountLine(); i++) {
			Vector2 start = m_pPlatformLine->GetStartPoint(i);
			Vector2 end = m_pPlatformLine->GetEndPoint(i);
			Vector2 mStart = pCollider_->GetPosition();
			Vector2 mEnd;
			mEnd.x = mStart.x;
			mEnd.y = mStart.y - pCollider_->GetScale().y * 0.5f;
			Vector2 result;
			// 아래와 선 검사
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
				isPlatform_ = true;		// 플랫폼 추가(언더점프에 사용할 예정)
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
	dashStart_ = false;	// 항상 땅검사 이후엔 false임
}


void Player::CollisionCheck()
{
}

void Player::InputUpdate()
{
	vector<Command*> commandQueue = inputHandler_->handleInput();	// 진짜로 큐는 아니야
	for (Command* command : commandQueue) {
 		GameActor* tempActor = this;
		command->execute(*tempActor);
	}
}

void Player::HandUpdate(Matrix V, Matrix P)
{
	Vector3 thisRotation = _animation->GetRotation();
	// 무기 안든 오른손
	float vec = std::abs(thisRotation.y / 180.0f);	// 절대값을 주어야 180 , -180에 대해 자유로워짐.
	vec *= 2.0f;
	vec -= 1.0f;	// 정방 -1 역방 1 방향 검증용
	float gap = 35.0f;
	hand_[1]->SetPosition(this->GetPosition().x + (vec * gap * WSCALEX), this->GetPosition().y - gap * WSCALEY);
	hand_[1]->Update(V, P);

	// 무기 든 왼손
	Vector3 afterRotation;
	Vector2 beforePosition;

	if (handedWeapon_[currentFocusHand_ * 2] != nullptr) {
		hand_[0]->SetPosition(handedWeapon_[currentFocusHand_ * 2]->GetLeftHandPoint());
		hand_[0]->SetRotation(handedWeapon_[currentFocusHand_ * 2]->GetLeftHandRotation());
		hand_[0]->_pivot = handedWeapon_[currentFocusHand_ * 2]->GetLeftHandPivot();
	}
	else	// 손 기본위치
	{
		hand_[0]->SetPosition(this->GetPosition().x - (vec * gap * 1.2f * WSCALEX), this->GetPosition().y - 20.0f * WSCALEY);
		float angle = Mouse->GetAngleRelativeToMouse(hand_[0]->GetPosition().x, hand_[0]->GetPosition().y);
		hand_[0]->SetRotation(0.0f, 0.0f, angle);
		hand_[0]->_pivot = Vector3(0.0f, 0.0f, 0.0f);
	}
	hand_[0]->Update(V, P);
}
// 각종 사이클을 위한 업데이트
void Player::CycleUpdate()
{
	dashCycleTime_ += TIMEMANAGER->Delta();
	if (dashCycleTime_ >= playerData_.dashRechargeTime) {
		dashCycleTime_ = 0.0f;
		DashRecharge(1);
	}
}
// GameActor 멤버함수로 변경
/*
void Player::GravityUpdate()
*/
void Player::UpdateHandedWeapon()
{
	// 무기의 소유자 표시해주기
	for (int i = 0; i < _countof(handedWeapon_); ++i)
		handedWeapon_[i] = nullptr;

	for (int i = 0; i < _countof(handedWeapon_); ++i) {
		handedWeapon_[i] = (Weapon*)Inventory_->GetEquipItem(i);
		if (handedWeapon_[i] != nullptr) {
			handedWeapon_[i]->SetOwner((GameActor*)this);	// 이중 포인터 비정상 작동으로 일반 포인터로 변경
		}
	}
}

void Player::Attacked(float damage)
{
//	eventHandler->Push(L"playerAttacked");	// 무기 구현같은거할때 필요하면 하자. 꼭 이벤트가 아닐수도 있다.
	float changed = max(damage - actorData_.armor, 1.0f);
	DecreaseHP((int)changed);
	Audio->Play("Hit_Player");
	ImmuneFrame_ = maxImmuneFrame_;
	//Explosion.wav 죽으면 이거 호출하면 됨.
}

void Player::Dash()
{
	if (playerData_.dashCount <= 0)
		return;

	Audio->Play("Dash_Player");
	// player의 Dash 일련의 과정 
	playerData_.dashCount--;	// 1. 카운트 감소
	dashRadian = Mouse->GetAngleRelativeToMouse(this->_position.x, this->_position.y, 1);	// 각도 구하기

	moveAmount = Vector2(cosf(dashRadian) * playerData_.baseSpeed * 2.0f * TIMEMANAGER->Delta() * WSCALEX,
		sinf(dashRadian) * playerData_.baseSpeed * 2.0f * TIMEMANAGER->Delta() * WSCALEY);
	dashStart_ = true;
	if (dashRadian >= 0.0f)
		moveAmount.y += 5.0f * WSCALEY;
	dashLifeCycle = 0.5f;	// 라이프사이클 초기화.
	dashCB = std::bind(&Player::DashDo, this);	// 이 콜백은 Update 쪽에서 실행됨.
	_currentState = State::JUMP;	
	// 이후 UI 변경
	playerUI->dashUI_->Dash();

}
// Dash 의 물리 애니메이션을 수행하며 lifeCycle 관리 
void Player::DashDo()
{
	isDash = true;
	// 이부분을 Dash() 에서 딱 한번 힘준다는 느낌으로 바꿈
//	this->ModifyPosition(cosf(dashRadian) * playerData_.baseSpeed* 1.5f * TIMEMANAGER->Delta() * WSCALEX,
//		sinf(dashRadian) * playerData_.baseSpeed * 1.5f * TIMEMANAGER->Delta() * WSCALEY);
	dashLifeCycle -= TIMEMANAGER->Delta();
	DashAnimationUpdate();

	if (dashLifeCycle <= 0.0f) {
		dashLifeCycle = 0.5f;	// 혹시 모르니 초기화
		dashCB = std::bind(&Player::DashWaiting, this);
		isDash = false;

		dashEffect_[0]->SetScale(0.0f, 0.0f);
		dashEffect_[1]->SetScale(0.0f, 0.0f);
		dashEffect_[2]->SetScale(0.0f, 0.0f);
	}
}

void Player::DashWaiting()
{
	// 얘는 아무것도 안함. 
}

// 0 1 2 3  01은 1번, 23은 2번
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
// 0 1 2 3  01은 1번, 23은 2번
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
		FatalBlow();	// 이건 actor 꺼임
}

void Player::Die()
{
	printf("PlayerDie\n");
}

// 왼쪽이 x 음수 moveSpeed x speed xy 만큼 이동하는 함수
void Player::LeftMove()
{
	if(isDash == false)
		moveAmount.x = -playerData_.baseSpeed * TIMEMANAGER->Delta() * WSCALEX;
	if (_currentState != State::JUMP)
		_currentState = State::RUN;

}
//	moveAmout를 변경시키는 방식
void Player::RightMove()
{
	if (isDash == false)
		moveAmount.x = playerData_.baseSpeed * TIMEMANAGER->Delta() * WSCALEX;
	if(_currentState != State::JUMP)
		_currentState = State::RUN;
}

void Player::DashRecharge(int amount)
{
	playerData_.dashCount = min(playerData_.maxDashCount, playerData_.dashCount + amount);	
	playerUI->dashUI_->IncreaseDashCount(amount);
}
// Next: 무식한 방법으로 애니메이션 제어, 여유 된다면 01, 12 20 순으로 index를 순회하며 놓아주기를 수행할 것
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
// 가변 프레임 지원 실패
void Player::Jump()
{
	// 점프 수행
	if (isGround_ == true) {
		isUnderJump_ = false;
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
	eventHandler->Push(L"SwapItem");
	UpdateHandedWeapon();
}
