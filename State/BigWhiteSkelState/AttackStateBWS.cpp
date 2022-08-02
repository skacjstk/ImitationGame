#include "ImitationGame/framework.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/BigWhiteSkelState/BigWhiteSkelState.h"
#include "State/BigWhiteSkelState/IdleStateBWS.h"
#include "AttackStateBWS.h"

void AttackStateBWS::SwitchState(BigWhiteSkel& actor)
{
	// 공격을 완료했다면, Next: 
	if (actor.GetAnimation()->IsPlay() == false) {
		actor.stateEnum_ = BigWhiteSkel::SkelState::IDLE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
}

void AttackStateBWS::Action(BigWhiteSkel& actor)
{
}

void AttackStateBWS::Enter(BigWhiteSkel& actor)
{
	actor.moveAmount = Vector2(0.0f, 0.0f);
	actor.GetAnimation()->SetPlay(static_cast<int>(actor.stateEnum_));
}
