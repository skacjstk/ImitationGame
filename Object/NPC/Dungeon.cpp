#include "ImitationGame/framework.h"
#include "Object/Player.h"
#include "Object/NPC.h"
#include "Dungeon.h"

Dungeon::Dungeon()
{
	SetActive(false);
	wstring shader = SHADER_FOLDER;	shader += L"TextureColor.hlsl";
	wstring strImage = IMAGE_FOLDER;	strImage += L"NPC/Dungeon/Eat/DungeonEat02.png";	// ���� ū �κ� �̹���
	_animation = new Animation(strImage, shader);


	// EAT
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::End);
		Texture* pTexture = _animation->GetTexture();
		for (int i = 0; i <= 27; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"NPC/Dungeon/Eat/DungeonEat" + to_wstring((int)(i * 0.1)) + to_wstring((int)(i % 10)) + L".png";
			pClip->AddFrame(pTexture, strImage, 0, 0, 0.075f);
			_animation->SetPivot(Vector3(0.0f, -85.0f * WSCALEY * 3.0f, 0.0f));
		}
		_animation->AddClip(pClip);
	}
	// INGURGITATE
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::End);
		Texture* pTexture = _animation->GetTexture();
		for (int i = 0; i <= 50; ++i) {
			printf("%d %d\n", (int)(i * 0.1), (int)(i % 10));
			strImage = IMAGE_FOLDER; strImage += L"NPC/Dungeon/Ingurgitate/DungeonIngurgitate" + to_wstring((int)(i * 0.1)) + to_wstring((int)(i % 10)) + L".png";
			pClip->AddFrame(pTexture, strImage, 0, 0, 0.1f);
		}
		_animation->AddClip(pClip);
	}

}

Dungeon::~Dungeon()
{
}

void Dungeon::Update(Matrix V, Matrix P)
{
	if (IsActive() == false)
		return;
	// Player�� �浹�ϸ� �ٷ� Communicate()
	switch (_currentState)
	{
	case DungeonState::HIDE:
		// Player���� �浹 �˻�
		CheckPlayer();
		break;
	case DungeonState::EAT:
	{
		_animation->SetPlay(0);
		Vector2 pos = _animation->GetAnimationClip(0)->GetCurrentFrameRealSize();
		_animation->SetPosition(this->GetPosition().x, this->GetPosition().y +
			_animation->GetAnimationClip( _animation->GetClipNo())->GetCurrentFrameRealSize().y * 0.5f  - (85 * GetScale().y * 0.5f));	// 85.�� ���� ū ���� ����
		_animation->Update(V, P);
		// �ִϸ��̼� ����� ���� �̵� ȣ��
		if (!_animation->IsPlay()) {
			eventHandler->Push(L"Eat");	// EatEvent ȣ��
			SetActive(false);
		}
	}
		break;
	default:
		break;
	}
}

void Dungeon::Render()
{
	if (IsActive() == false)
		return;
	_animation->Render();
}

void Dungeon::Reset()
{
	SetActive(true);
	_currentState = DungeonState::HIDE;
	ppPlayer = (Player*)OBJECTMANAGER->FindObject("player");
	this->SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
	_animation->SetScale(this->GetScale());
}

void Dungeon::Communicate()
{
}

void Dungeon::CheckPlayer()
{
	Vector2 tempPosition = (*ppPlayer).GetPosition();
	float x = this->_position.x - tempPosition.x;
	float y = this->_position.y - tempPosition.y;

	if (fabsf(x) < 100.0f && fabsf(y) < 65.0f * WSCALEY)
	{
		_currentState = DungeonState::EAT;
		this->_position.x = tempPosition.x;
	}
}