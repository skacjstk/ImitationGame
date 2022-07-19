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
		//actor.state_->enter(*this);	// å�� �ִ� ����
	}
}

void IdleState::Action(SkelDog& actor)
{
	printf("IdleState ����\n");
	duringTime += TIMEMANAGER->Delta();
	// ���� ���⼭ �ִϸ��̼� ��ȣ�� ��������: ���� ������ Enter ���

}
