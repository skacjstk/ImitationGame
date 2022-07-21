#include "ImitationGame/framework.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/BigWhiteSkelState/BigWhiteSkelState.h"
#include "State/BigWhiteSkelState/RunStateBWS.h"
#include "WaitStateBWS.h"

void WaitStateBWS::SwitchState(BigWhiteSkel& actor)
{
	// 일정 거리 이내라면, RUN State로 변경
	if (time_ > 1.0f) {
		actor.stateEnum_ = BigWhiteSkel::SkelState::ATTACK;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
}

void WaitStateBWS::Action(BigWhiteSkel& actor)
{
	time_ += TIMEMANAGER->Delta();
}

void WaitStateBWS::Enter(BigWhiteSkel& actor)
{
	time_ = 0.0f;
}
