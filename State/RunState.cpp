#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState.h"
#include "State/IdleState.h"
#include "RunState.h"

void RunState::SwitchState(SkelDog& actor)
{
	if (duringTime > 1.0f) {
		duringTime = 0.0f;
		// IDLE ���� ��ü
	
		actor.currentState_ = actor.state_[0];

		//actor.state_->enter(*this);	// å�� �ִ� ����
	}	
}

void RunState::Action(SkelDog& actor)
{
	printf("RunState ����\n"); 
	duringTime += TIMEMANAGER->Delta();
	// ���� ���⼭ �ִϸ��̼� ��ȣ�� ��������: ���� ������ Enter ���
}
