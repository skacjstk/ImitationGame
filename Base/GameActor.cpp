#include "ImitationGame/framework.h"
#include "Object/Line.h"
#include "Physics/Collider.h"
#include "GameActor.h"

void GameActor::GroundCheck()
{
	Scene* tempScene = SCENEMANAGER->GetCurrentScene();
	Line* m_pGroundLine = tempScene->GetGroundLines();
	Line* m_pCeilingLine = tempScene->GetCeilingLines();
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
			SetY(charPos.y - 1.0f * WSCALEY);
			flag = true;
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
		Vector2 AreaMin = Vector2(pCollider_->GetPosition().x - size.x * 0.5f, 
			pCollider_->GetPosition().y - size.y * 0.5f);
		Vector2 AreaMax = Vector2(pCollider_->GetPosition().x + size.x * 0.5f, 
			pCollider_->GetPosition().y + size.y * 0.5f);

		if (Line::Clipping(start, end, AreaMin, AreaMax))
		{
			float fMinX = min(start.x, end.x);
			float fMaxX = max(start.x, end.x);
			float fMinY = min(start.y, end.y);
			float fMaxY = max(start.y, end.y);

			if (fMaxX < charPos.x && fMinX < charPos.x)
				charPos.x = fMaxX + size.x * 0.5f;
			else
				charPos.x = fMinX - size.x * 0.5f;

			if (fMaxY > charPos.y && fMinY > charPos.y)
			{
				float Slope = (end.y - start.y) / (end.x - start.x);
				charPos.y = fMaxY - size.y * 0.5f;
			}
			SetPosition(charPos);
			isConflicted_ = flag;
			break;
		}
	}
	isGround_ = flag;
}

void GameActor::GravityUpdate()
{
	if (isGround_ == true)
		gravity_ = 0.0f;
	else
		gravity_ -= G * TIMEMANAGER->Delta() * 1.5f;
	ModifyPosition(0.0f, gravity_);
	if (gravity_ > 0.0f) {
		isFall = false;
	}
	else if (gravity_ < 0.0f) {
		isFall = true;
		isJump = false;
	}
	beforeGround_ = isGround_;
}
// 임시코드
void GameActor::Attacked()
{
	actorData_.HP -= 1;
}
