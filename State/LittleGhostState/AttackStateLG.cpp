#include "ImitationGame/framework.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/LittleGhostState/LittleGhostState.h"
#include "State/LittleGhostState/IdleStateLG.h"
#include "AttackStateLG.h"

void AttackStateLG::SwitchState(LittleGhost& actor)
{
	// 공격을 완료했다면, Next: 
	if (time_ > 2.0f) {
		actor.stateEnum_ = LittleGhost::LittleGhostStateEnum::MOVE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
}

void AttackStateLG::Action(LittleGhost& actor)
{
	time_ += TIMEMANAGER->Delta();
	actor.Attack(radian, dX, dY);
}

void AttackStateLG::Enter(LittleGhost& actor)
{
	time_ = 0.0f;
	actor.GetAnimation()->SetPlay(static_cast<int>(actor.stateEnum_));

	dX = actor.chaseTarget_->GetPosition().x - actor.GetPosition().x;
	dY = actor.chaseTarget_->GetPosition().y - actor.GetPosition().y;
	// 2제곱을 더한 값의 제곱근 = 거리
	radian = atan2(dY, dX);
}
