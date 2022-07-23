#include "ImitationGame/framework.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/BigWhiteSkelState/BigWhiteSkelState.h"
#include "State/BigWhiteSkelState/IdleStateBWS.h"
#include "MoveStateBWS.h"

void RunStateBWS::SwitchState(BigWhiteSkel& actor)
{
	float x = pow((actor.GetPosition().x - actor.chaseTarget_->GetPosition().x), 2);
	float y = pow((actor.GetPosition().y - actor.chaseTarget_->GetPosition().y), 2);
	// 2제곱을 더한 값의 제곱근 = 거리
	distance = sqrtf(x + y);
	// 이동 거리가 안변하거나, 추적객체와의 거리가 멀면서 60도 이상이면 JUMP 수행
	if (actor.IsConflicted()) {
		actor.stateEnum_ = BigWhiteSkel::SkelState::JUMP;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	// 일정 거리 바깥이라면, IDLE 로 변경
	else if (distance > 700.0f * WSCALEX) {
		actor.stateEnum_ = BigWhiteSkel::SkelState::IDLE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	// 일정 거리 이내라면, WAIT 으로 변경 ( 이후, Attack )
	else if (distance < 100.0f * WSCALEX) {
		actor.stateEnum_ = BigWhiteSkel::SkelState::WAIT;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
}

void RunStateBWS::Action(BigWhiteSkel& actor)
{
	beforePos = actor.GetPosition();
	actor.Move();

	actor.GroundCheck();
	actor.GravityUpdate();
}

void RunStateBWS::Enter(BigWhiteSkel& actor)
{
	actor.GetAnimation()->SetPlay(static_cast<int>(actor.stateEnum_));
}
