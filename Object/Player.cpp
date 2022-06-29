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
	
	// ����
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

// ������ x ���� moveSpeed x speed xy ��ŭ �̵��ϴ� �Լ�
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
// �ش� ��ġ�� �����̷��� �õ��ϸ� ��ġ�� �ٲ�( �� ��ġ�� �߷� �� ��Ÿ ���𰡿� ���� �ٲ� �� ����)
void Player::Move(Vector2& position)
{
	// Collider ���� �浹 �����ϱ�
	SetPosition(position);
}

void Player::Jump()
{
	Vector2 position = this->GetPosition();

	position.y += playerData_.baseSpeed * TIMEMANAGER->Delta();

	SetPosition(position);
}
