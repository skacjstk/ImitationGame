#include "ImitationGame/framework.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/BigWhiteSkelState/BigWhiteSkelState.h"
#include "State/BigWhiteSkelState/RunStateBWS.h"
#include "JumpStateBWS.h"

void JumpStateBWS::SwitchState(BigWhiteSkel& actor)
{
	// ���� ���� ��, IDLE �� ����
	if (actor.IsGround() == true) {
		actor.stateEnum_ = BigWhiteSkel::SkelState::IDLE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	//actor.state_->enter(*this);	// å�� �ִ� ����	
}

void JumpStateBWS::Action(BigWhiteSkel& actor)
{
	// �ϴ°� ���µ�, ���������� �ʰ�...
	actor.Jump();
}

void JumpStateBWS::Enter(BigWhiteSkel& actor)
{
	// ��Ʈ���� ����
}
