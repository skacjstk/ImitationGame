#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState/SkelDogState.h"
#include "State/SkelDogState/IdleState.h"
#include "RunState.h"

void RunState::SwitchState(SkelDog& actor)
{
	float x = pow((actor.GetPosition().x - actor.chaseTarget_->GetPosition().x), 2);
	float y = pow((actor.GetPosition().y - actor.chaseTarget_->GetPosition().y), 2);
	// 2������ ���� ���� ������ = �Ÿ�
	distance = sqrtf(x + y);
	// �̵� �Ÿ��� �Ⱥ��ϰų�, ������ü���� �Ÿ��� �ָ鼭 60�� �̻��̸� JUMP ����
	if (actor.IsConflicted()) {
		actor.stateEnum_ = SkelDog::DogState::JUMP;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	// ���� �Ÿ� �ٱ��̶��, IDLE �� ����
	else if (distance > 700.0f * WSCALEX) {
		actor.stateEnum_ = SkelDog::DogState::IDLE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	// ���� �Ÿ� �̳����, WAIT ���� ���� ( ����, Attack )
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
