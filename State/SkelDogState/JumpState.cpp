#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState/SkelDogState.h"
#include "State/SkelDogState/IdleState.h"
#include "JumpState.h"

void JumpState::SwitchState(SkelDog& actor)
{
	// ���� ���� ��, IDLE �� ����
	if (actor.IsGround() == true) {
		actor.stateEnum_ = SkelDog::DogState::IDLE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	//actor.state_->enter(*this);	// å�� �ִ� ����	
}

void JumpState::Action(SkelDog& actor)
{
	// �ϴ°� ���µ�, ���������� �ʰ�...
	actor.Jump();
}

void JumpState::Enter(SkelDog& actor)
{
	// ��Ʈ���� ����
}
