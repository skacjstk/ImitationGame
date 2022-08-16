#include "ImitationGame/framework.h"
#include "Object/Player.h"
#include "Object/NPC.h"
#include "Dungeon.h"

Dungeon::Dungeon()
{
	SetActive(false);
	wstring shader = SHADER_FOLDER;	shader += L"TextureColor.hlsl";
	wstring strImage = IMAGE_FOLDER;	strImage += L"NPC/Dungeon/Eat/DungeonEat02.png";	// 제일 큰 부분 이미지
	_animation = new Animation(strImage, shader);


	// EAT
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::End);
		Texture* pTexture = _animation->GetTexture();
		for (int i = 0; i <= 27; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"NPC/Dungeon/Eat/DungeonEat" + to_wstring((int)(i * 0.1)) + to_wstring((int)(i % 10)) + L".png";
			pClip->AddFrame(pTexture, strImage, 0, 0, 0.075f);
		}
		_animation->AddClip(pClip);
		_animation->GetAnimationClip(0)->SetClipPivot(Vector3(0.0f, -85.0f * WSCALEY * 3.0f, 0.0f));
	}
	// INGURGITATE
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::End);
		Texture* pTexture = _animation->GetTexture();
		for (int i = 0; i <= 50; ++i) {
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

	if (isRise_ == true) {
		FadeEffect();
		return;
	}
	// Player와 충돌하면 바로 Communicate()
	switch (_currentState)
	{
	case DungeonState::HIDE:
		// Player와의 충돌 검사
		CheckPlayer();
		break;
	case DungeonState::EAT:
	{
		Audio->Stop("Town");
		_animation->SetPlay(0);

		if (_animation->GetAnimationClip(0)->GetCurrentFrame() == 0 && playSound2_ == false) {	// 처음에 이거
			Audio->Play("DungeonOut");
			playSound2_ = true;
		}
		else if (_animation->GetAnimationClip(0)->GetCurrentFrame() == 9 && playSound_ == false)	// 던전 닫힐때
		{
			ppPlayer->SetState(Player::State::EAT);
			Audio->Play("syExplo1");
			Audio->Play("earthquake1");
			playSound_ = true;
		}

		Vector2 pos = _animation->GetAnimationClip(0)->GetCurrentFrameRealSize();
		_animation->SetPosition(this->GetPosition().x, this->GetPosition().y +
			_animation->GetAnimationClip( _animation->GetClipNo())->GetCurrentFrameRealSize().y * 0.5f  - (85 * GetScale().y * 0.5f));	// 85.는 제일 큰 사진 기준
		_animation->Update(V, P);


		// 애니메이션 종료시 던전 이동 호출
		if (!_animation->IsPlay()) {
			isRise_ = true;
			Audio->Stop("earthquake1");
			fadeTimeCheck_ = 0.0f;
			CAMERA->FadeOut(1.5f);
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
	playSound_ = false;
	playSound2_ = false;
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

void Dungeon::FadeEffect()
{
	fadeTimeCheck_ += TIMEMANAGER->Delta();
	if (fadeTimeCheck_ >= 1.5f) {
		eventHandler->Push(L"EnterDungeon");	// EatEvent 호출
		SetActive(false);
	}
}
