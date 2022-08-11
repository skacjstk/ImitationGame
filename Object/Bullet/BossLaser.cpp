#include "ImitationGame/framework.h"
#include "Physics/Collider.h"
#include "Object/Enemy/SkellBoss/SkellBoss.h"
#include "BossLaser.h"

BossLaser::BossLaser()
{
	SetActive(false);
	wstring strImage = IMAGE_FOLDER; strImage += L"Boss/SkellBoss/SkellBossLaserHead0.png";
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";
	laserHead_ = new Animation(strImage, strShader);

	// Head
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::End);

		for (int i = 0; i <= 6; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Boss/SkellBoss/SkellBossLaserHead" + to_wstring(i) + L".png";
			pClip->AddFrame(laserHead_->GetTexture(), strImage, 0, 0, 0.06f);

		}
		laserHead_->AddClip(pClip);
	}
	strImage = IMAGE_FOLDER; strImage += L"Boss/SkellBoss/SkellBossLaserBody0.png";
	for (UINT k = 0; k < _countof(laserBody_); ++k) {
		laserBody_[k] = new Animation(strImage, strShader);
		{
			AnimationClip* pClip = new AnimationClip(AnimationClip::eState::End);

			for (int i = 0; i <= 6; ++i) {
				strImage = IMAGE_FOLDER; strImage += L"Boss/SkellBoss/SkellBossLaserBody" + to_wstring(i) + L".png";
				pClip->AddFrame(laserBody_[k]->GetTexture(), strImage, 0, 0, 0.06f);
			}
			laserBody_[k]->AddClip(pClip);
		}
	}	


	pCollider_ = new Collider();

}

BossLaser::~BossLaser()
{
	SAFE_DELETE(laserHead_);
	for (UINT i = 0; i < _countof(laserBody_); ++i) {
		SAFE_DELETE(laserBody_[i]);
	}
	owner_ = nullptr;
}				

void BossLaser::Update(Matrix V, Matrix P)
{
	if (IsActive() == false)
		return;
		
	// ���¸� ����ȭ�ϱ� �����ؼ� �� if��
	if (stateEnum_ == BLState::FIRE)
		Fire(V, P);
	else if (stateEnum_ == BLState::END)
		Stop(V, P);

}

void BossLaser::Render()
{
	if (IsActive() == false)
		return;


	laserHead_->Render();
	for (UINT i = 0; i < _countof(laserBody_); ++i)
		laserBody_[i]->Render();

	pCollider_->Render();
}

void BossLaser::Reset()
{
	SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
	laserHead_->SetScale(GetScale());

	for (UINT i = 0; i < _countof(laserBody_); ++i)
		laserBody_[i]->SetScale(GetScale());
	//Reset�� ���� �ʼ����� ��������.

}

void BossLaser::SummonLaser(SkellBoss** owner, Vector2 handPos, HandDirection direction, GameActor* chaseTarget)
{
	owner_ = owner;
	chaseTarget_ = chaseTarget;
	// Right �������� 0��
	Vector2 laserGap = Vector2(0.0f, 0.0f);

	laserHead_->SetPosition(handPos);
	laserGap.x = laserBody_[0]->GetTextureRealSize().x;	// �̰� ��ġ ������ �Ǿ��� ����
	// ���⼭ body�� ���ػ������, 5�ȼ� ������. ( 2.5 )
	laserHead_->SetPlay(0);
	if (direction == HandDirection::RIGHT) {
		laserHead_->SetRotation(0.0f, 180.0f, 0.0f);
		laserHead_->ModifyPosition(-2.5f * laserHead_->GetScale().x, 0.0f);		// head ��ġ�� �������ֱ�
		laserGap.x *= -1;	// ��ġ�� �ٲ��ֱ�
		for (UINT i = 0; i < _countof(laserBody_); ++i) {
			laserBody_[i]->SetPlay(0);
			laserBody_[i]->SetRotation(0.0f, 180.0f, 0.0f);
			laserBody_[i]->SetPosition(handPos + Vector2(laserGap.x * (i + 1), laserGap.y));
		}
	}// Right�� ���� �ٲ��ֱ�
	else {
		laserHead_->ModifyPosition(2.5f * laserHead_->GetScale().x, 0.0f);		// head ��ġ�� �������ֱ�
		laserHead_->SetRotation(0.0f, 0.0f, 0.0f);
		for (UINT i = 0; i < _countof(laserBody_); ++i) {
			laserBody_[i]->SetPlay(0);
			laserBody_[i]->SetRotation(0.0f, 0.0f, 0.0f);
			laserBody_[i]->SetPosition(handPos + Vector2(laserGap.x * (i + 1), laserGap.y));
		}
	}
	// ��ġ
	Matrix V, P;
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();
	// �ѹ��� ������Ʈ ����� �׷��ȹ��� �Ȼ���.
	laserHead_->Update(V, P);
	for (UINT i = 0; i < _countof(laserBody_); ++i)
		laserBody_[i]->Update(V, P);

	Vector2 cXY = Vector2(laserBody_[0]->GetTextureRealSize());
	cXY.x *= (_countof(laserBody_) + 1);	//
	pCollider_->SetScale(cXY);	// �� �Ҹ���? x ũ�⸸ 32*7 �̶��
	pCollider_->SetPosition(laserBody_[_countof(laserBody_) / 2 -1]->GetPosition());	// �̷л� 7���� 1 + 3 ��°�� �갡 �߾�
	pCollider_->Update(V, P);

	// ���� �ٲ��ֱ�
	stateEnum_ = BLState::FIRE;
	Audio->Play("iceball");
	SetActive(true);
}

void BossLaser::Fire(Matrix V, Matrix P)
{
	laserHead_->Update(V, P);
	for (UINT i = 0; i < _countof(laserBody_); ++i) 
		laserBody_[i]->Update(V, P);
	pCollider_->Update(V, P);

	if (laserHead_->GetAnimationClip(0)->GetCurrentFrame() <= 3)
		CheckAttack();
	// �ִϸ��̼� Ŭ����ȣ 4������ �浹������ ������ ����.

	if (laserHead_->IsPlay() == false)
		stateEnum_ = BLState::END;
}

void BossLaser::Stop(Matrix V, Matrix P)
{
	pCollider_->SetScale(0.0f, 0.0f);
	pCollider_->Update(V, P);
	SetActive(false);
	(*owner_)->DecreaseLaserCount();
}

void BossLaser::CheckAttack()
{
	//������ ���� �浹ü�� Obb�� 
	if (chaseTarget_->GetImmuneFrame() <= 0 && Collider::IntersectAABB(pCollider_, chaseTarget_->GetCollider())) {
		chaseTarget_->Attacked(damage_);		
	}
}
