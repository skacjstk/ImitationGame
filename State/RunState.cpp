#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState.h"
#include "State/IdleState.h"
#include "RunState.h"

void RunState::SwitchState(SkelDog& actor)
{
	float x = pow((actor.GetPosition().x - actor.chaseTarget_->GetPosition().x), 2);
	float y = pow((actor.GetPosition().y - actor.chaseTarget_->GetPosition().y), 2);
	// 2������ ���� ���� ������ = �Ÿ�
	distance = sqrtf(x + y);
	// ���� �Ÿ� �ٱ��̶��, IDLE �� ����
	if (distance > 700.0f * WSCALEX) {
		actor.stateEnum_ = SkelDog::DogState::IDLE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	// ���� �Ÿ� �̳����, ATTACK ���� ����
	else if (distance < 100.0f * WSCALEX) {
		actor.stateEnum_ = SkelDog::DogState::ATTACK;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
}

void RunState::Action(SkelDog& actor)
{
	// chaseTarget_ ����
	printf("�ذ� RUN\n");
	float dX = actor.chaseTarget_->GetPosition().x - actor.GetPosition().x;
	float sign = copysign(1, dX);	// copysign ��ȣ ��Ʈ ���� ��¶�� ����� 1, ������ -1
	actor.ModifyPosition(sign * actor.moveSpeed * TIMEMANAGER->Delta(), 0.0f);	// Next: -1 ���ؾ� �� �� ����.
	
	sign -= 1.0f;	// ��� 0, ���� -2
	actor.SetRotation(0.0f, sign * 90.0f, 0.0f);

	actor.GravityUpdate();
}

void RunState::Enter(SkelDog& actor)
{
}
