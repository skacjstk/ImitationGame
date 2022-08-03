#include "ImitationGame/framework.h"
#include "Object/Line.h"
#include "Physics/Collider.h"
#include "GameActor.h"

void GameActor::EffectUpdate(Matrix V, Matrix P)
{
	// 몬스터 공통 이펙트 전용
	// 죽어가는 동안 이펙트 출력
	if (actorData_.living == GameActor::ActorState::DYING) {
		// 이펙트 가져오는건 Dying()을 호출할때 함.
//		if (dieEffect_ != nullptr)
		(*dieEffect_)->Update(V, P);
		SetActive(false);	// 이렇게 해야 이펙트만 갱신됨.
		if ((*dieEffect_)->IsPlay() == false) {
			dieEffect_ = nullptr;	//이걸 null로 바꿔주어야 함. Dying할때 새로 가져오고
			Die();
		}
	}
}

void GameActor::EffectRender()
{
	if (dieEffect_ != nullptr)
		(*dieEffect_)->Render();
}

// PlatformLine 추가해야 함. + currentScene뿐만 아니라 Room 변경도....
void GameActor::GroundCheck()
{
	Scene* tempScene = SCENEMANAGER->GetCurrentScene();
	Line* m_pGroundLine = tempScene->GetGroundLines();
	Line* m_pCeilingLine = tempScene->GetCeilingLines();
	Line* m_pPlatformLine = tempScene->GetPlatformLines();
	bool flag = false;
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
			Vector2 charPos = GetPosition();
			float fRad = atan2f(end.y - start.y, end.x - start.x);
			float Slope = (end.y - start.y) / (end.x - start.x);
			charPos.y = Slope * (charPos.x - start.x) + start.y + _animation->GetTextureRealSize().y * 0.5f;
		
			SetY(charPos.y);
			flag = true;
			isJump = false;
			isFall = false;
			break;
		}
	}//end for

	// 플랫폼라인 충돌검사 ( 대시중에는 플랫폼 충돌 불가: 나중에)

		for (int i = 0; i < m_pPlatformLine->GetCountLine(); i++) {
			Vector2 start = m_pPlatformLine->GetStartPoint(i);
			Vector2 end = m_pPlatformLine->GetEndPoint(i);
			Vector2 mStart = pCollider_->GetPosition();
			Vector2 mEnd;
			mEnd.x = mStart.x;
			mEnd.y = mStart.y - pCollider_->GetScale().y * 0.5f;
			Vector2 result;
			// 아래와 선 검사
			if (!isJump && Line::IntersectionLine(start, end, mStart, mEnd, result))
			{
				Vector2 charPos = GetPosition();
				float fRad = atan2f(end.y - start.y, end.x - start.x);
				float Slope = (end.y - start.y) / (end.x - start.x);
				charPos.y = Slope * (charPos.x - start.x) + start.y + _animation->GetTextureRealSize().y * 0.5f;
			
				SetY(charPos.y);
				isGround_ = true;
				flag = true;
				isPlatform_ = true;		
				// isground
				isJump = false;
				isFall = false;
				break;
			}
		}//end for
	
	for (UINT i = 0; i < m_pCeilingLine->GetCountLine(); i++)
	{
		isConflicted_ = false;
		Vector2 start = m_pCeilingLine->GetStartPoint(i);
		Vector2 end = m_pCeilingLine->GetEndPoint(i);

		Vector2 charPos = GetPosition();
		Vector2 size = pCollider_->GetScale();
		Vector2 left = Vector2(charPos.x - size.x * 0.5f, charPos.y);
		Vector2 right = Vector2(charPos.x + size.x * 0.5f, charPos.y);
		Vector2 top = Vector2(charPos.x, charPos.y + (size.y * 0.5f));
		Vector2 result;

		if (isConflicted_ = Collider::InterSectionLine(charPos, right, start, end)) {
			moveAmount.x = -0.5f * WSCALEX;
			isConflicted_ = true;
			break;
		}
		if (isConflicted_ = Collider::InterSectionLine(charPos, left, start, end)) {
			moveAmount.x = +0.5f * WSCALEX;
			isConflicted_ = true;
			break;
		}
		if (isConflicted_ = Collider::InterSectionLine(charPos, top, start, end)) {
			moveAmount.y = -0.5f * WSCALEY;
			isConflicted_ = true;
			break;
		}
	}
	isGround_ = flag;
}
// moveAmout 변경중
void GameActor::GravityUpdate()
{
	if (isGround_ == true)
		moveAmount.y = beforeGravity_ = gravity_ = 0.0f;
	else {
		gravity_ -= G * TIMEMANAGER->Delta() * 1.5f * WSCALEY;
		moveAmount.y += (gravity_ - beforeGravity_);
	}
	beforeGravity_ = gravity_;
	if (gravity_ > 0.0f) {
		isFall = false;
	}
	else if (gravity_ < 0.0f) {
		isFall = true;
		isJump = false;
	}
	beforeGround_ = isGround_;
}
// 재정의자 제외 모두 이거 쓰려고.
void GameActor::Attacked(float damage)
{
	float changed = max(damage - actorData_.armor, 1.0f);
	actorData_.HP -= (int)changed;
	Audio->Play("Hit_Monster");
	// HitEffect 가져와야 함.
	HPChange();
}
void GameActor::FatalBlow()
{
	Dying();
}
// 그저 신호기 역할.
void GameActor::HPChange()
{
	// 오브젝트마다 만들어줘야 함.
	//	playerUI->playerLifeUI_->UpdateLifeBar(actorData_.HP, actorData_.maxHP);
	int hp = (int)actorData_.HP;
	if (hp <= 0)
		FatalBlow();
}
void GameActor::Move(Vector2& position)
{
	ModifyPosition(position);
}
void GameActor::Dying()
{
	Audio->Play("MonsterDie");
	actorData_.living = ActorState::DYING;
	dieEffect_ = objectPool->GetMonsterDieEffect();
	(*dieEffect_)->SetPlay(0);
	(*dieEffect_)->SetPosition(GetPosition());
	(*dieEffect_)->SetScale(GetScale());
}
void GameActor::Die()
{
	actorData_.living = ActorState::DIE;
	eventHandler->Push(L"EnemyDie");	// 현재 Room 에 속한 적의 수를 -1 하는 것. 0이 되면 맵은 clear 상태가 되고, 그때 문이 열림.
	SetActive(false);
}

