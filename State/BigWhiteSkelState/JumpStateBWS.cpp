#include "ImitationGame/framework.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/BigWhiteSkelState/BigWhiteSkelState.h"
#include "State/BigWhiteSkelState/MoveStateBWS.h"
#include "JumpStateBWS.h"

void JumpStateBWS::SwitchState(BigWhiteSkel& actor)
{
	// 점프 수행 후, IDLE 로 변경
	if (actor.IsGround() == true) {
		actor.stateEnum_ = BigWhiteSkel::SkelState::IDLE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	//actor.state_->enter(*this);	// 책에 있던 내용	
}

void JumpStateBWS::Action(BigWhiteSkel& actor)
{
	// 하는게 없는데, 움직이지도 않고...
	actor.Jump();

	actor.GroundCheck();
	actor.GravityUpdate();
}

void JumpStateBWS::Enter(BigWhiteSkel& actor)
{
	actor.GetAnimation()->SetPlay(static_cast<int>(actor.stateEnum_));
	// 히트판정 제거
}
