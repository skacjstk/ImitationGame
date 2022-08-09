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
	// �ݶ��̴��� ������ ��� 13 * 0.707�� ������

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
		// Dying���� ���� ����
		// Next: ����� �׽�Ʈ������ active�� ����
		SetActive(false);
	}

	ModifyPosition(movePos);	// GO
	_animation->ModifyPosition(movePos);	// Animation ( ���� ������, �����ϴ� ���� ��ġ�� �ٸ� )
	_animation->Update(V, P);
	// ���� ����
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
	pCollider_->SetScale(_animation->GetTextureRealSize() * 0.707f);	// ���簢�� ������ ũ����.
	actorData_.living = ActorState::LIVE;
}

void BossBullet::Fire(float radian, Vector2& skelMousePos, GameActor* chaseTarget)
{
	chaseTarget_ = chaseTarget;
	SetPosition(skelMousePos);
	_animation->SetPosition(skelMousePos);	// ������ ������� �̰� �ٲ��� ��
	_animation->SetPlay(0);
	movePos = Vector2(cosf(radian) * moveSpeed * TIMEMANAGER->Delta(), sinf(radian) * moveSpeed * TIMEMANAGER->Delta());

	lifeframe_ = TIMEMANAGER->GetFrame() * 2;
	SetActive(true);
}

void BossBullet::CheckAttack()
{
	if (chaseTarget_->GetImmuneFrame() <= 0 && Collider::IntersectAABB(pCollider_, chaseTarget_->GetCollider())) {
		chaseTarget_->Attacked(bulletDamage_);
		// �ڱ� �ڽ��� ���� ��
		Dying();
	}
}

void BossBullet::EffectUpdate(Matrix V, Matrix P)
{	// �׾�� ���� ����Ʈ ���
	if (actorData_.living == GameActor::ActorState::DYING) {
		// ����Ʈ �������°� Dying()�� ȣ���Ҷ� ��.
		if (bulletFX_ != nullptr) {
			(*bulletFX_)->Update(V, P);
			SetActive(false);	// �̷��� �ؾ� ����Ʈ�� ���ŵ�.
		}
		if (bulletFX_ != nullptr && (*bulletFX_)->IsPlay() == false) {
			bulletFX_ = nullptr;	//�̰� null�� �ٲ��־�� ��. Dying�Ҷ� ���� ��������
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
