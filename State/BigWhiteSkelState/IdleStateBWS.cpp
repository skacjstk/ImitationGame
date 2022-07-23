#include "ImitationGame/framework.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/BigWhiteSkelState/BigWhiteSkelState.h"
#include "State/BigWhiteSkelState/MoveStateBWS.h"
#include "IdleStateBWS.h"

void IdleStateBWS::SwitchState(BigWhiteSkel& actor)
{
	float x = pow((actor.GetPosition().x - actor.chaseTarget_->GetPosition().x), 2);
	float y = pow((actor.GetPosition().y - actor.chaseTarget_->GetPosition().y), 2);
	// 2제곱을 더한 값의 제곱근 = 거리
	distance = sqrtf(x + y);
	// 일정 거리 이내라면, RUN State로 변경
	if (fabsf(distance) < 700.0f * WSCALEX) {
		actor.stateEnum_ = BigWhiteSkel::SkelState::MOVE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	//actor.state_->enter(*this);	// 책에 있던 내용	
}

void IdleStateBWS::Action(BigWhiteSkel& actor)
{
	// 하는게 없는데, 움직이지도 않고...

	actor.GroundCheck();
	actor.GravityUpdate();
}

void IdleStateBWS::Enter(BigWhiteSkel& actor)
{
	actor.GetAnimation()->SetPlay(static_cast<int>(actor.stateEnum_));
	// 히트판정 제거
}
