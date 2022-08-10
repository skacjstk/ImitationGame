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
		
	// 상태를 패턴화하긴 간단해서 걍 if문
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
	//Reset은 단지 필수조정 같은거임.

}

void BossLaser::SummonLaser(SkellBoss** owner, Vector2 handPos, HandDirection direction, GameActor* chaseTarget)
{
	owner_ = owner;
	chaseTarget_ = chaseTarget;
	// Right 기준으로 0도
	Vector2 laserGap = Vector2(0.0f, 0.0f);

	laserHead_->SetPosition(handPos);
	laserGap.x = laserBody_[0]->GetTextureRealSize().x;	// 이게 위치 변수가 되어줄 예정
	// 여기서 body를 기준삼았으니, 5픽셀 부족함. ( 2.5 )
	laserHead_->SetPlay(0);
	if (direction == HandDirection::RIGHT) {
		laserHead_->SetRotation(0.0f, 180.0f, 0.0f);
		laserHead_->ModifyPosition(-2.5f * laserHead_->GetScale().x, 0.0f);		// head 위치값 보정해주기
		laserGap.x *= -1;	// 설치값 바꿔주기
		for (UINT i = 0; i < _countof(laserBody_); ++i) {
			laserBody_[i]->SetPlay(0);
			laserBody_[i]->SetRotation(0.0f, 180.0f, 0.0f);
			laserBody_[i]->SetPosition(handPos + Vector2(laserGap.x * (i + 1), laserGap.y));
		}
	}// Right면 각도 바꿔주기
	else {
		laserHead_->ModifyPosition(2.5f * laserHead_->GetScale().x, 0.0f);		// head 위치값 보정해주기
		laserHead_->SetRotation(0.0f, 0.0f, 0.0f);
		for (UINT i = 0; i < _countof(laserBody_); ++i) {
			laserBody_[i]->SetPlay(0);
			laserBody_[i]->SetRotation(0.0f, 0.0f, 0.0f);
			laserBody_[i]->SetPosition(handPos + Vector2(laserGap.x * (i + 1), laserGap.y));
		}
	}
	// 설치
	Matrix V, P;
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();
	// 한번은 업데이트 해줘야 그래픽버그 안생김.
	laserHead_->Update(V, P);
	for (UINT i = 0; i < _countof(laserBody_); ++i)
		laserBody_[i]->Update(V, P);

	Vector2 cXY = Vector2(laserBody_[0]->GetTextureRealSize());
	cXY.x *= (_countof(laserBody_) + 1);	//
	pCollider_->SetScale(cXY);	// 뭔 소리냐? x 크기만 32*7 이라고
	pCollider_->SetPosition(laserBody_[_countof(laserBody_) / 2 -1]->GetPosition());	// 이론상 7개중 1 + 3 번째인 얘가 중앙
	pCollider_->Update(V, P);

	// 상태 바꿔주기
	stateEnum_ = BLState::FIRE;
	Audio->Play("BossLaser");
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
	// 애니메이션 클립번호 4번부턴 충돌판정을 구하지 않음.

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
	//각도를 가진 충돌체라 Obb로 
	if (chaseTarget_->GetImmuneFrame() <= 0 && Collider::IntersectAABB(pCollider_, chaseTarget_->GetCollider())) {
		chaseTarget_->Attacked(damage_);		
	}
}
