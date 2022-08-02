#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState/SkelDogState.h"
#include "State/SkelDogState/RunState.h"
#include "WaitState.h"

void WaitState::SwitchState(SkelDog& actor)
{
	// 일정 거리 이내라면, RUN State로 변경
	if (time_ > 1.0f) {
		actor.stateEnum_ = SkelDog::DogState::ATTACK;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
}

void WaitState::Action(SkelDog& actor)
{
	time_ += TIMEMANAGER->Delta();
}

void WaitState::Enter(SkelDog& actor)
{
	actor.moveAmount.x = 0.0f;
	time_ = 0.0f;
}
