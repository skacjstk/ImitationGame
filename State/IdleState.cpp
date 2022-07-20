#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState.h"
#include "State/RunState.h"
#include "IdleState.h"

void IdleState::SwitchState(SkelDog& actor)
{
	float x = pow((actor.GetPosition().x - actor.chaseTarget_->GetPosition().x), 2);
	float y = pow((actor.GetPosition().y - actor.chaseTarget_->GetPosition().y), 2);
	// 2������ ���� ���� ������ = �Ÿ�
	distance = sqrtf(x + y);
	// ���� �Ÿ� �̳����, RUN State�� ����
	if (fabsf(distance) < 700.0f * WSCALEX) {
		actor.stateEnum_ = SkelDog::DogState::RUN;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	//actor.state_->enter(*this);	// å�� �ִ� ����	
}

void IdleState::Action(SkelDog& actor)
{
	actor.GravityUpdate();
	// �ϴ°� ���µ�, ���������� �ʰ�...
}

void IdleState::Enter(SkelDog& actor)
{
	// ��Ʈ���� ����
}
