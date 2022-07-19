#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState.h"
#include "State/IdleState.h"
#include "RunState.h"

void RunState::SwitchState(SkelDog& actor)
{
	if (duringTime > 1.0f) {
		duringTime = 0.0f;
		// IDLE 으로 교체
	
		actor.currentState_ = actor.state_[0];

		//actor.state_->enter(*this);	// 책에 있던 내용
	}	
}

void RunState::Action(SkelDog& actor)
{
	printf("RunState 실행\n"); 
	duringTime += TIMEMANAGER->Delta();
	// 나는 여기서 애니메이션 번호를 수정하자: 상태 패턴의 Enter 대용
}
