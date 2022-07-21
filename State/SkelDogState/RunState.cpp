#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState/SkelDogState.h"
#include "State/SkelDogState/IdleState.h"
#include "RunState.h"

void RunState::SwitchState(SkelDog& actor)
{
	float x = pow((actor.GetPosition().x - actor.chaseTarget_->GetPosition().x), 2);
	float y = pow((actor.GetPosition().y - actor.chaseTarget_->GetPosition().y), 2);
	// 2제곱을 더한 값의 제곱근 = 거리
	distance = sqrtf(x + y);
	// 이동 거리가 안변하거나, 추적객체와의 거리가 멀면서 60도 이상이면 JUMP 수행
	if (actor.IsConflicted()) {
		actor.stateEnum_ = SkelDog::DogState::JUMP;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	// 일정 거리 바깥이라면, IDLE 로 변경
	else if (distance > 700.0f * WSCALEX) {
		actor.stateEnum_ = SkelDog::DogState::IDLE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	// 일정 거리 이내라면, WAIT 으로 변경 ( 이후, Attack )
	else if (distance < 100.0f * WSCALEX) {
		actor.stateEnum_ = SkelDog::DogState::WAIT;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
}

void RunState::Action(SkelDog& actor)
{
	beforePos = actor.GetPosition();
	actor.Move();
}

void RunState::Enter(SkelDog& actor)
{
}
