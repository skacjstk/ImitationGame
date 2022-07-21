#include "ImitationGame/framework.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/BigWhiteSkelState/BigWhiteSkelState.h"
#include "State/BigWhiteSkelState/RunStateBWS.h"
#include "IdleStateBWS.h"

void IdleStateBWS::SwitchState(BigWhiteSkel& actor)
{
	float x = pow((actor.GetPosition().x - actor.chaseTarget_->GetPosition().x), 2);
	float y = pow((actor.GetPosition().y - actor.chaseTarget_->GetPosition().y), 2);
	// 2������ ���� ���� ������ = �Ÿ�
	distance = sqrtf(x + y);
	// ���� �Ÿ� �̳����, RUN State�� ����
	if (fabsf(distance) < 700.0f * WSCALEX) {
		actor.stateEnum_ = BigWhiteSkel::SkelState::MOVE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	//actor.state_->enter(*this);	// å�� �ִ� ����	
}

void IdleStateBWS::Action(BigWhiteSkel& actor)
{
	// �ϴ°� ���µ�, ���������� �ʰ�...
}

void IdleStateBWS::Enter(BigWhiteSkel& actor)
{
	// ��Ʈ���� ����
}
