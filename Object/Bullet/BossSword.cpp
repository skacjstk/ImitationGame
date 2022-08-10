#include "ImitationGame/framework.h"
#include "Physics/Collider.h"
#include "Object/Enemy/SkellBoss/SkellBoss.h"
#include "BossSword.h"

BossSword::BossSword()
{
	// Į ������ ����Ʈ�� ���� ����.
	// charge�� �������� ����� �� 
	// �߻�! �Ҷ� FX ���·�
	// ���߸� HitFX ������ ���� �� false
	SetActive(false);
	wstring strImage = IMAGE_FOLDER; strImage += L"Particle/BossSword/BossSword.png";
	wstring strShader = SHADER_FOLDER; strShader += L"Texture.hlsl";

	bossSword_[0] = new Texture(strImage, strShader);
	_animation = new Animation(strImage, strShader);
	strImage = IMAGE_FOLDER; strImage += L"Particle/BossSword/BossSwordFX.png";
	bossSword_[1] = new Texture(strImage, strShader);	// 1���� ���Ͼ� ����(�߻���)
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
	// �ݶ��̴��� ������ ��� 13 * 0.707�� ������

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
		
	// ���¸� ����ȭ�ϱ� �����ؼ� �� if��
	if (stateEnum_ == BState::CHASE)
		Chasing(V, P);
	else if (stateEnum_ == BState::FIRE)
		Fire(V, P);
	else if (stateEnum_ == BState::STOP)
		Stop(V, P);

	// Chasing() or Fire(), Fire �Ҷ� �굵 groundCHeck�� STOP���� ������ �����ð� ���� false
	// ���� ����
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

	swordDyingFrame = TIMEMANAGER->GetFrame() * 2;	// 2���� �ױ�

	actorData_.living = ActorState::LIVE;
}

void BossSword::SummonSword(SkellBoss** owner, Vector2& summonerPos, float waitTime, GameActor* chaseTarget)
{
	owner_ = owner;
	chaseTarget_ = chaseTarget;
	swordActiveIndex = 0;
	SetPosition(summonerPos);
	_animation->SetPosition(summonerPos);	// ������� 0���� ��� �� �ϳ�
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
	GroundCheck();	// BossSword ���� �߰�
	if (isConflicted_ == true) {
	//	moveAmount = Vector2(0.0f,0.0f);
		stateEnum_ = BState::STOP;
		swordActiveIndex = 0;
		bossSword_[swordActiveIndex]->SetPosition(this->_position);
		chaseRad_ -= 90.0f;	// �ٽ� ���� �̻��ϰ� 
		bossSword_[swordActiveIndex]->SetRotation(0.0f, 0.0f, chaseRad_);
		bossSword_[swordActiveIndex]->Update(V, P);	// �ѹ� ����� �׷����� ����
		chaseRad_ += 90.0f;	// �ٽ� ���� �������� 
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
	chaseRad_ += (PI * 0.5f);	// 90���� �߰��ϱ� ���� �̻��ϰ�
	chaseRad_ = chaseRad_ / PI * 180.0f;
	bossSword_[swordActiveIndex]->SetPosition(GetPosition());
	this->SetRotation(0.0f, 0.0f, chaseRad_);
	bossSword_[swordActiveIndex]->SetRotation(0.0f, 0.0f, chaseRad_);
	bossSword_[swordActiveIndex]->Update(V, P);

	pCollider_->SetPosition(GetPosition());
	pCollider_->SetScale(bossSword_[0]->GetTextureRealSize() * 0.8f);
	pCollider_->SetRotation(0.0f, 0.0f, chaseRad_);
	pCollider_->Update(V, P);


	// �̰� charge�ִϸ��̼� ������ ����
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
		bossSword_[swordActiveIndex]->Update(V, P);	// �ѹ� ��������� �׷����� ����
		chaseRad_ += 90.0f;	// ���� ����ȭ 
	}
}


void BossSword::CheckAttack()
{
	//������ ���� �浹ü�� Obb�� 
	if (chaseTarget_->GetImmuneFrame() <= 0 && Collider::Obb(pCollider_, chaseTarget_->GetCollider())) {
		chaseTarget_->Attacked(bulletDamage_);		
	}
}

void BossSword::EffectUpdate(Matrix V, Matrix P)
{	// �׾�� ���� ����Ʈ ���
	if (actorData_.living == GameActor::ActorState::DYING) {
		// ����Ʈ �������°� Dying()�� ȣ���Ҷ� ��.
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

	// �ٴڿ� �� ������ ����Ʈ�� ����� ����
	hitGroundFX_->SetPlay(0);
	hitGroundFX_->SetScale(this->_scale);
	hitGroundFX_->SetPosition(this->_position);
	// Next: ��, õ�忡 ���� ���� 90�� ������ �����ϰ�, ��ġ ������ �׷��� �ϱ�

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


// platform �浹�� �˻����� ����. ��, �浹�Ұ�� moveAmount�� 0���� ������. ( �̰� GrounCheck �ٷ� ���� Update�� ���� )
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
		// �Ʒ��� �� �˻�
		if (!isJump && Line::IntersectionLine(start, end, mStart, mEnd, result))
		{
			isGround_ = true;
			isConflicted_ = true;
			break;
		}
	}//end for

	// �浹�ߴٸ� ���� �� �˻��� �ʿ� ����.
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
