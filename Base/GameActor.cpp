#include "ImitationGame/framework.h"
#include "Object/Line.h"
#include "Physics/Collider.h"
#include "GameActor.h"

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
			break;
		}
		if (isConflicted_ = Collider::InterSectionLine(charPos, left, start, end)) {
			moveAmount.x = +0.5f * WSCALEX;
			break;
		}
		if (isConflicted_ = Collider::InterSectionLine(charPos, top, start, end)) {
			moveAmount.y = -0.5f * WSCALEY;
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
	actorData_.HP -= changed;
	HPChange();
}
void GameActor::FatalBlow()
{
	Die();
}
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
void GameActor::Die()
{
	printf("액터 사망\n");
}

