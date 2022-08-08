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
	// �ݶ��̴��� ������ ��� 13 * 0.707�� ������

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
		// Dying���� ���� ����
		// Next: ����� �׽�Ʈ������ active�� ����
		SetActive(false);
	}

	ModifyPosition(movePos);	// GO
	_animation->ModifyPosition(movePos);	// Animation ( ���� ������, �����ϴ� ���� ��ġ�� �ٸ� )
	_animation->Update(V, P);
	// ���� ����
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
	_animation->SetPosition(skelMousePos);	// ������ ������� �̰� �ٲ��� ��
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
