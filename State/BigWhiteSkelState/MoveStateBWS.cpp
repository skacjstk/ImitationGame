#include "ImitationGame/framework.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/BigWhiteSkelState/BigWhiteSkelState.h"
#include "State/BigWhiteSkelState/IdleStateBWS.h"
#include "MoveStateBWS.h"

void RunStateBWS::SwitchState(BigWhiteSkel& actor)
{
	float x = pow((actor.GetPosition().x - actor.chaseTarget_->GetPosition().x), 2);
	float y = pow((actor.GetPosition().y - actor.chaseTarget_->GetPosition().y), 2);
	// 2������ ���� ���� ������ = �Ÿ�
	distance = sqrtf(x + y);
	// �̵� �Ÿ��� �Ⱥ��ϰų�, ������ü���� �Ÿ��� �ָ鼭 60�� �̻��̸� JUMP ����
	if (actor.IsConflicted()) {
		actor.stateEnum_ = BigWhiteSkel::SkelState::JUMP;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	// ���� �Ÿ� �ٱ��̶��, IDLE �� ����
	else if (distance > 700.0f * WSCALEX) {
		actor.stateEnum_ = BigWhiteSkel::SkelState::IDLE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	// ���� �Ÿ� �̳����, WAIT ���� ���� ( ����, Attack )
	else if (distance < 100.0f * WSCALEX) {
		actor.stateEnum_ = BigWhiteSkel::SkelState::WAIT;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
}

void RunStateBWS::Action(BigWhiteSkel& actor)
{
	beforePos = actor.GetPosition();
	actor.Move();

	actor.GroundCheck();
	actor.GravityUpdate();
}

void RunStateBWS::Enter(BigWhiteSkel& actor)
{
	actor.GetAnimation()->SetPlay(static_cast<int>(actor.stateEnum_));
}
