#include "ImitationGame/framework.h"
#include "Object/Line.h"
#include "Physics/Collider.h"
#include "GameActor.h"

void GameActor::GroundCheck()
{
	Scene* tempScene = SCENEMANAGER->GetCurrentScene();
	Line* m_pGroundLine = tempScene->GetLines();
	bool flag = false;
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
		// 머리와 선 검사
		mEnd.x = mStart.x;
		mEnd.y = mStart.y + pCollider_->GetScale().y * 0.5f;
		if (Line::IntersectionLine(start, end, mStart, mEnd, result)) {
			mStart.y = result.y - pCollider_->GetScale().y * 0.5f;
			SetY(mStart.y);
			isConflicted_ = true;
			break;

		}
		// 왼쪽과 선 검사
		mEnd.x = mStart.x - pCollider_->GetScale().x * 0.5f;
		mEnd.y = mStart.y;
		if (Line::IntersectionLine(start, end, mStart, mEnd, result)) {
			mStart.x = result.x - pCollider_->GetScale().x * 0.5f;
			isConflicted_ = true;
			break;
		}
		// 오른쪽과 선 검사
		mEnd.x = mStart.x + pCollider_->GetScale().x * 0.5f;
		mEnd.y = mStart.y;
		if (Line::IntersectionLine(start, end, mStart, mEnd, result)) {
			mStart.x = result.x - pCollider_->GetScale().x * 0.5f;
			isConflicted_ = true;
			break;
		}
	}
	isGround_ = flag;
}

void GameActor::GravityUpdate()
{
	if (isGround_ == true)
		gravity_ = 0.0f;
//	else if (beforeGround_ != isGround_ && isJump == false)
//		gravity_ = -1.5f;
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
