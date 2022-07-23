#include "ImitationGame/framework.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/LittleGhostState/LittleGhostState.h"
#include "State/LittleGhostState/MoveStateLG.h"
#include "IdleStateLG.h"

void IdleStateLG::SwitchState(LittleGhost& actor)
{
	float x = pow((actor.GetPosition().x - actor.chaseTarget_->GetPosition().x), 2);
	float y = pow((actor.GetPosition().y - actor.chaseTarget_->GetPosition().y), 2);
	// 2제곱을 더한 값의 제곱근 = 거리
	distance = sqrtf(x + y);
	// 일정 거리 이내라면, Move State로 변경
	if (fabsf(distance) < 700.0f * WSCALEX) {
		actor.stateEnum_ = LittleGhost::LittleGhostStateEnum::MOVE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	//actor.state_->enter(*this);	// 책에 있던 내용	
}

void IdleStateLG::Action(LittleGhost& actor)
{
	// 하는게 없는데, 움직이지도 않고...

	actor.GroundCheck();
	actor.GravityUpdate();
}

void IdleStateLG::Enter(LittleGhost& actor)
{
	actor.GetAnimation()->SetPlay(static_cast<int>(actor.stateEnum_));
	// 히트판정 제거
}
