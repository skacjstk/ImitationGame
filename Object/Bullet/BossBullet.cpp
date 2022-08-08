#include "ImitationGame/framework.h"
#include "BossBullet.h"

BossBullet::BossBullet()
{
	SetActive(false);
	wstring strImage = IMAGE_FOLDER; strImage += L"Particle/BossBullet000.png";
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";

	_animation = new Animation(strImage, strShader);

	// IDLE
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::Loop);

		for (int i = 0; i < 2; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Particle/BossBullet00" + to_wstring(i) + L".png";
			pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.06f);

		}
		_animation->AddClip(pClip);
	}
	// 콜라이더를 제곱근 대신 13 * 0.707을 곱하자

}

BossBullet::~BossBullet()
{
	SAFE_DELETE(_animation);
}

void BossBullet::Update(Matrix V, Matrix P)
{
	if (IsActive() == false)
		return;

	// AttackCheck();
	
	--lifeframe_;
	if (lifeframe_ <= 0) {
		// Dying으로 상태 변경
		// Next: 현재는 테스트용으로 active만 변경
		SetActive(false);
	}

	ModifyPosition(movePos);	// GO
	_animation->ModifyPosition(movePos);	// Animation ( 둘이 같지만, 저장하는 변수 위치가 다름 )
	_animation->Update(V, P);
	// 생성 시점
}

void BossBullet::Render()
{
	if (IsActive() == false)
		return;
	_animation->Render();
}

void BossBullet::Reset()
{
	SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
	_animation->SetScale(GetScale());
}

void BossBullet::Fire(float radian, Vector2& skelMousePos, GameActor* chaseTarget)
{
	chaseTarget_ = chaseTarget;
	SetPosition(skelMousePos);
	_animation->SetPosition(skelMousePos);	// 포인터 사라지면 이거 바꾸지 뭐
	_animation->SetPlay(0);
	movePos = Vector2(cosf(radian) * moveSpeed, sinf(radian) * moveSpeed);

	lifeframe_ = TIMEMANAGER->GetFrame() * 2;
	SetActive(true);
}

void BossBullet::Dying()
{
}

void BossBullet::Die()
{
}
