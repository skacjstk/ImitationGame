#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState.h"
#include "State/RunState.h"
#include "IdleState.h"

void IdleState::SwitchState(SkelDog& actor)
{
	if (duringTime > 1.0f) {
		duringTime = 0.0f;
		actor.currentState_ = actor.state_[1];
		//actor.state_->enter(*this);	// 책에 있던 내용
	}
}

void IdleState::Action(SkelDog& actor)
{
	printf("IdleState 실행\n");
	duringTime += TIMEMANAGER->Delta();
	// 나는 여기서 애니메이션 번호를 수정하자: 상태 패턴의 Enter 대용

}
