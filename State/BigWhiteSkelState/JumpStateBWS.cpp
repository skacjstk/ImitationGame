#include "ImitationGame/framework.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/BigWhiteSkelState/BigWhiteSkelState.h"
#include "State/BigWhiteSkelState/MoveStateBWS.h"
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

	actor.GroundCheck();
	actor.GravityUpdate();
}

void JumpStateBWS::Enter(BigWhiteSkel& actor)
{
	actor.GetAnimation()->SetPlay(static_cast<int>(actor.stateEnum_));
	// ��Ʈ���� ����
}
