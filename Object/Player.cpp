#include "ImitationGame/framework.h"
#include "./Base/InputHandler.h"
#include "./Command/Command.h"
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
	SetScale(5.0f, 5.0f);
}

Player::~Player()
{
	SAFE_DELETE(_animation);
}


void Player::Update(Matrix V, Matrix P)
{
	Vector2 position = GetPosition();
	
	// 적용
	SetPosition(position);

	InputUpdate();

	_animation->SetPlay(_currentState + _heroType);
	_animation->SetRotation(GetRotation());
	_animation->SetPosition(GetPosition());
	_animation->SetScale(GetScale());
	 
	_animation->Update(V, P);
	CAMERA->Update(V, P);

}

void Player::InputUpdate()
{
	Command* command = inputHandler.handleInput();
	if (command) {
		GameActor* tempActor = this;
		command->execute(*tempActor);
	}
}

void Player::Render()
{
	_animation->Render();
}

void Player::Reset()
{
	_currentState = State::IDLE;
}

// 왼쪽이 x 음수 moveSpeed x speed xy 만큼 이동하는 함수
void Player::LeftMove()
{
	Vector2 position = this->GetPosition();

	position.x -= playerData_.baseSpeed * TIMEMANAGER->Delta();
	Move(position);
}

void Player::RightMove()
{
	Vector2 position = this->GetPosition();

	position.x += playerData_.baseSpeed * TIMEMANAGER->Delta();
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
	Vector2 position = this->GetPosition();

	position.y += playerData_.baseSpeed * TIMEMANAGER->Delta();

	SetPosition(position);
}
