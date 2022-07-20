#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState.h"
#include "State/RunState.h"
#include "WaitState.h"

void WaitState::SwitchState(SkelDog& actor)
{
	// ���� �Ÿ� �̳����, RUN State�� ����
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
	time_ = 0.0f;
}
