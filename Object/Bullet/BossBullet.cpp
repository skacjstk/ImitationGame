#include "ImitationGame/framework.h"
#include "Physics/Collider.h"
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
	pCollider_ = new Collider();
	// 콜라이더를 제곱근 대신 13 * 0.707을 곱하자

}

BossBullet::~BossBullet()
{
	SAFE_DELETE(_animation);
}

void BossBullet::Update(Matrix V, Matrix P)
{
	EffectUpdate(V, P);
	if (IsActive() == false)
		return;
	
	CheckAttack();
	
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
	pCollider_->SetPosition(GetPosition());
	pCollider_->Update(V, P);
}

void BossBullet::Render()
{
	EffectRender();
	if (IsActive() == false)
		return;
	_animation->Render();
	pCollider_->Render();
}

void BossBullet::Reset()
{
	SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
	_animation->SetScale(GetScale());
	pCollider_->SetScale(_animation->GetTextureRealSize() * 0.707f);	// 정사각형 제곱근 크기임.
	actorData_.living = ActorState::LIVE;
}

void BossBullet::Fire(float radian, Vector2& skelMousePos, GameActor* chaseTarget)
{
	chaseTarget_ = chaseTarget;
	SetPosition(skelMousePos);
	_animation->SetPosition(skelMousePos);	// 포인터 사라지면 이거 바꾸지 뭐
	_animation->SetPlay(0);
	movePos = Vector2(cosf(radian) * moveSpeed * TIMEMANAGER->Delta(), sinf(radian) * moveSpeed * TIMEMANAGER->Delta());

	lifeframe_ = TIMEMANAGER->GetFrame() * 2;
	SetActive(true);
}

void BossBullet::CheckAttack()
{
	if (chaseTarget_->GetImmuneFrame() <= 0 && Collider::IntersectAABB(pCollider_, chaseTarget_->GetCollider())) {
		chaseTarget_->Attacked(bulletDamage_);
		// 자기 자신을 죽일 것
		Dying();
	}
}

void BossBullet::EffectUpdate(Matrix V, Matrix P)
{	// 죽어가는 동안 이펙트 출력
	if (actorData_.living == GameActor::ActorState::DYING) {
		// 이펙트 가져오는건 Dying()을 호출할때 함.
		if (bulletFX_ != nullptr) {
			(*bulletFX_)->Update(V, P);
			SetActive(false);	// 이렇게 해야 이펙트만 갱신됨.
		}
		if (bulletFX_ != nullptr && (*bulletFX_)->IsPlay() == false) {
			bulletFX_ = nullptr;	//이걸 null로 바꿔주어야 함. Dying할때 새로 가져오고
			Die();
		}
	}
}

void BossBullet::EffectRender()
{
	if (bulletFX_ != nullptr)
		(*bulletFX_)->Render();
}

void BossBullet::Dying()
{
	actorData_.living = ActorState::DYING;
	bulletFX_ = objectPool->GetBossBulletEffect();
	(*bulletFX_)->SetPlay(0);
	(*bulletFX_)->SetPosition(GetPosition());
	(*bulletFX_)->SetScale(GetScale());
}

void BossBullet::Die()
{
	bulletFX_ = nullptr;
	actorData_.living = ActorState::DIE;
	SetActive(false);
}
