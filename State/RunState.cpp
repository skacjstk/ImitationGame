#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState.h"
#include "State/IdleState.h"
#include "RunState.h"

void RunState::SwitchState(SkelDog& actor)
{
	float x = pow((actor.GetPosition().x - actor.chaseTarget_->GetPosition().x), 2);
	float y = pow((actor.GetPosition().y - actor.chaseTarget_->GetPosition().y), 2);
	// 2제곱을 더한 값의 제곱근 = 거리
	distance = sqrtf(x + y);
	// 일정 거리 바깥이라면, IDLE 로 변경
	if (distance > 700.0f * WSCALEX) {
		actor.stateEnum_ = SkelDog::DogState::IDLE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	// 일정 거리 이내라면, ATTACK 으로 변경
	else if (distance < 100.0f * WSCALEX) {
		actor.stateEnum_ = SkelDog::DogState::ATTACK;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
}

void RunState::Action(SkelDog& actor)
{
	// chaseTarget_ 추적
	printf("해골개 RUN\n");
	float dX = actor.chaseTarget_->GetPosition().x - actor.GetPosition().x;
	float sign = copysign(1, dX);	// copysign 부호 비트 추출 어쨋든 양수면 1, 음수면 -1
	actor.ModifyPosition(sign * actor.moveSpeed * TIMEMANAGER->Delta(), 0.0f);	// Next: -1 곱해야 할 수 있음.
	
	sign -= 1.0f;	// 양수 0, 음수 -2
	actor.SetRotation(0.0f, sign * 90.0f, 0.0f);

	actor.GravityUpdate();
}

void RunState::Enter(SkelDog& actor)
{
}
