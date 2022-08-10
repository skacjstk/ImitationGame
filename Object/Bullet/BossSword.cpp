#include "ImitationGame/framework.h"
#include "Physics/Collider.h"
#include "Object/Enemy/SkellBoss/SkellBoss.h"
#include "BossSword.h"

BossSword::BossSword()
{
	// 칼 등장은 이펙트가 따로 있음.
	// charge를 역순으로 재생한 후 
	// 발사! 할땐 FX 상태로
	// 멈추면 HitFX 나오고 정지 후 false
	SetActive(false);
	wstring strImage = IMAGE_FOLDER; strImage += L"Particle/BossSword/BossSword.png";
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";

	bossSword_[0] = new Texture(strImage, strShader);
	_animation = new Animation(strImage, strShader);
	strImage = IMAGE_FOLDER; strImage += L"Particle/BossSword/BossSwordFX.png";
	bossSword_[1] = new Texture(strImage, strShader);	// 1번이 새하얀 상태(발사중)
	// chargeAni
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::End);

		for (int i = 0; i <= 7; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Particle/BossSword/BossSwordCharge" + to_wstring(i) + L".png";
			pClip->AddFrame(_animation->GetTexture(), strImage, 0, 0, 0.06f);

		}
		_animation->AddClip(pClip);
	}

	// 
	strImage = IMAGE_FOLDER; strImage += L"Particle/BossSword/BossSwordHitFX0.png";
	hitGroundFX_ = new Animation(strImage, strShader);
	// hitFX
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::eState::End);

		for (int i = 0; i <= 4; ++i) {
			strImage = IMAGE_FOLDER; strImage += L"Particle/BossSword/BossSwordHitFX" + to_wstring(i) + L".png";
			pClip->AddFrame(hitGroundFX_->GetTexture(), strImage, 0, 0, 0.06f);

		}
		hitGroundFX_->AddClip(pClip);
	}


	pCollider_ = new Collider();
	// 콜라이더를 제곱근 대신 13 * 0.707을 곱하자

}

BossSword::~BossSword()
{
	SAFE_DELETE(_animation);
}

void BossSword::Update(Matrix V, Matrix P)
{
	EffectUpdate(V, P);
	if (IsActive() == false)
		return;
		
	// 상태를 패턴화하긴 간단해서 걍 if문
	if (stateEnum_ == BState::CHASE)
		Chasing(V, P);
	else if (stateEnum_ == BState::FIRE)
		Fire(V, P);
	else if (stateEnum_ == BState::STOP)
		Stop(V, P);

	// Chasing() or Fire(), Fire 할땐 얘도 groundCHeck로 STOP으로 변경후 일정시간 이후 false
	// 생성 시점
}

void BossSword::Render()
{
	EffectRender();
	if (IsActive() == false)
		return;
	bossSword_[swordActiveIndex]->Render();

	if(_animation->IsPlay() == true)
		_animation->Render();

	pCollider_->Render();
}

void BossSword::Reset()
{
	SetScale(6.0f * WSCALEX, 6.0f * WSCALEY);
	_animation->SetScale(GetScale());
	bossSword_[0]->SetScale(GetScale());
	bossSword_[1]->SetScale(GetScale());

	swordDyingFrame = TIMEMANAGER->GetFrame() * 2;	// 2초후 죽기

	actorData_.living = ActorState::LIVE;
}

void BossSword::SummonSword(SkellBoss** owner, Vector2& summonerPos, float waitTime, GameActor* chaseTarget)
{
	owner_ = owner;
	chaseTarget_ = chaseTarget;
	swordActiveIndex = 0;
	SetPosition(summonerPos);
	_animation->SetPosition(summonerPos);	// 차지모션 0번에 든거 딱 하나
	bossSword_[0]->SetPosition(summonerPos);
	bossSword_[1]->SetPosition(summonerPos);
	pCollider_->SetPosition(summonerPos);
	stateEnum_ = BState::CHASE;
	waitTime_ = waitTime;
	SetActive(true);
}

void BossSword::Fire(Matrix V, Matrix P)
{
	swordActiveIndex = 1;
	movePos = Vector2(cosf(chaseRad_ / 180.0f * PI) * moveSpeed * TIMEMANAGER->Delta() * WSCALEX, sinf(chaseRad_ / 180.0f * PI) * moveSpeed * TIMEMANAGER->Delta() * WSCALEY);
	ModifyPosition(movePos);
	bossSword_[swordActiveIndex]->SetPosition(this->_position);
	bossSword_[swordActiveIndex]->Update(V, P);

	pCollider_->SetPosition(this->_position);
	pCollider_->SetRotation(this->_rotation);
	pCollider_->Update(V, P);

	CheckAttack();
	GroundCheck();	// BossSword 전용 추가
	if (isConflicted_ == true) {
	//	moveAmount = Vector2(0.0f,0.0f);
		stateEnum_ = BState::STOP;
		swordActiveIndex = 0;
		bossSword_[swordActiveIndex]->SetPosition(this->_position);
		chaseRad_ -= 90.0f;	// 다시 각도 이상하게 
		bossSword_[swordActiveIndex]->SetRotation(0.0f, 0.0f, chaseRad_);
		bossSword_[swordActiveIndex]->Update(V, P);	// 한번 해줘야 그래픽이 예쁨
		chaseRad_ += 90.0f;	// 다시 각도 정상으로 
		Dying();
	}
}

void BossSword::Stop(Matrix V, Matrix P)
{
	--swordDyingFrame;
	bossSword_[swordActiveIndex]->Update(V, P);
	pCollider_->Update(V, P);
}

void BossSword::Chasing(Matrix V, Matrix P)
{
	waitTime_ -= TIMEMANAGER->Delta();
	Vector2 tempPosition = this->GetPosition();

	float fdX = tempPosition.x - chaseTarget_->GetPosition().x;
	float fdY = tempPosition.y - chaseTarget_->GetPosition().y;
	chaseRad_ = atan2f(fdY, fdX);
	chaseRad_ += (PI * 0.5f);	// 90도를 추가하기 각도 이상하게
	chaseRad_ = chaseRad_ / PI * 180.0f;
	bossSword_[swordActiveIndex]->SetPosition(GetPosition());
	this->SetRotation(0.0f, 0.0f, chaseRad_);
	bossSword_[swordActiveIndex]->SetRotation(0.0f, 0.0f, chaseRad_);
	bossSword_[swordActiveIndex]->Update(V, P);

	pCollider_->SetPosition(GetPosition());
	pCollider_->SetScale(bossSword_[0]->GetTextureRealSize() * 0.8f);
	pCollider_->SetRotation(0.0f, 0.0f, chaseRad_);
	pCollider_->Update(V, P);


	// 이거 charge애니메이션 프레임 개임
	if (waitTime_ < (8 * 0.06f)) {
		_animation->SetPlay(0);
		_animation->SetPosition(GetPosition());
		_animation->SetRotation(0.0f, 0.0f, chaseRad_);
		_animation->Update(V, P);
	}
	if (waitTime_ <= 0.001f) {
		stateEnum_ = BState::FIRE;
		swordActiveIndex = 1;
		_animation->SetStop();
		bossSword_[swordActiveIndex]->SetPosition(GetPosition());
		bossSword_[swordActiveIndex]->SetRotation(0.0f, 0.0f, chaseRad_);
		bossSword_[swordActiveIndex]->Update(V, P);	// 한번 갱신해줘야 그래픽이 예쁨
		chaseRad_ += 90.0f;	// 각도 정상화 
	}
}


void BossSword::CheckAttack()
{
	//각도를 가진 충돌체라 Obb로 
	if (chaseTarget_->GetImmuneFrame() <= 0 && Collider::Obb(pCollider_, chaseTarget_->GetCollider())) {
		chaseTarget_->Attacked(bulletDamage_);		
	}
}

void BossSword::EffectUpdate(Matrix V, Matrix P)
{	// 죽어가는 동안 이펙트 출력
	if (actorData_.living == GameActor::ActorState::DYING) {
		// 이펙트 가져오는건 Dying()을 호출할때 함.
		if (hitGroundFX_ != nullptr) {
			hitGroundFX_->Update(V, P);
		}
		if (swordDyingFrame <= 0) {
			Die();
		}
	}
}

void BossSword::EffectRender()
{
	if (hitGroundFX_->IsPlay())
		hitGroundFX_->Render();
}

void BossSword::Dying()
{
	actorData_.living = ActorState::DYING;

	// 바닥에 검 꽂히는 이펙트의 모양을 조절
	hitGroundFX_->SetPlay(0);
	hitGroundFX_->SetScale(this->_scale);
	hitGroundFX_->SetPosition(this->_position);
	// Next: 벽, 천장에 따라 각도 90도 단위로 조절하고, 위치 조절도 그렇게 하기

	hitGroundFX_->ModifyPosition(0.0f, -pCollider_->GetScale().y * 0.3f);
	Matrix V, P;
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();
	hitGroundFX_->Update(V, P);
}

void BossSword::Die()
{
	actorData_.living = ActorState::DIE;
	(*owner_)->DecreaseSword();
	SetActive(false);
}


// platform 충돌을 검사하지 않음. 단, 충돌할경우 moveAmount를 0으로 고정함. ( 이건 GrounCheck 바로 이후 Update에 있음 )
void BossSword::GroundCheck()
{
	Scene* tempScene = SCENEMANAGER->GetCurrentScene();
	Line* m_pGroundLine = tempScene->GetGroundLines();
	Line* m_pCeilingLine = tempScene->GetCeilingLines();
	isGround_ = false;
	isConflicted_ = false;
	for (UINT i = 0; i < m_pGroundLine->GetCountLine(); i++) {
		Vector2 start = m_pGroundLine->GetStartPoint(i);
		Vector2 end = m_pGroundLine->GetEndPoint(i);
		Vector2 mStart = pCollider_->GetPosition();
		Vector2 mEnd;
		mEnd.x = mStart.x;
		mEnd.y = mStart.y - pCollider_->GetScale().y * 0.5f;
		Vector2 result;
		// 아래와 선 검사
		if (!isJump && Line::IntersectionLine(start, end, mStart, mEnd, result))
		{
			isGround_ = true;
			isConflicted_ = true;
			break;
		}
	}//end for

	// 충돌했다면 굳이 또 검사할 필요 없다.
	if (isConflicted_ == false) {
		for (UINT i = 0; i < m_pCeilingLine->GetCountLine(); i++)
		{
			Vector2 start = m_pCeilingLine->GetStartPoint(i);
			Vector2 end = m_pCeilingLine->GetEndPoint(i);

			Vector2 charPos = GetPosition();
			Vector2 size = pCollider_->GetScale();
			Vector2 left = Vector2(charPos.x - size.x * 0.5f, charPos.y);
			Vector2 right = Vector2(charPos.x + size.x * 0.5f, charPos.y);
			Vector2 top = Vector2(charPos.x, charPos.y + (size.y * 0.5f));
			Vector2 result;

			if (isConflicted_ = Collider::InterSectionLine(charPos, right, start, end)) {
				isConflicted_ = true;
				break;
			}
			if (isConflicted_ = Collider::InterSectionLine(charPos, left, start, end)) {
				isConflicted_ = true;
				break;
			}
			if (isConflicted_ = Collider::InterSectionLine(charPos, top, start, end)) {
				isConflicted_ = true;
				break;
			}
		}
	}// end for
}
