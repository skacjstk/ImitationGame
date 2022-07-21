#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState/SkelDogState.h"
#include "State/SkelDogState/IdleState.h"
#include "JumpState.h"

void JumpState::SwitchState(SkelDog& actor)
{
	// 점프 수행 후, IDLE 로 변경
	if (actor.IsGround() == true) {
		actor.stateEnum_ = SkelDog::DogState::IDLE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	//actor.state_->enter(*this);	// 책에 있던 내용	
}

void JumpState::Action(SkelDog& actor)
{
	// 하는게 없는데, 움직이지도 않고...
	actor.Jump();
}

void JumpState::Enter(SkelDog& actor)
{
	// 히트판정 제거
}
