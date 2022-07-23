#include "ImitationGame/framework.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/LittleGhostState/LittleGhostState.h"
#include "State/LittleGhostState/MoveStateLG.h"
#include "IdleStateLG.h"

void IdleStateLG::SwitchState(LittleGhost& actor)
{
	float x = pow((actor.GetPosition().x - actor.chaseTarget_->GetPosition().x), 2);
	float y = pow((actor.GetPosition().y - actor.chaseTarget_->GetPosition().y), 2);
	// 2������ ���� ���� ������ = �Ÿ�
	distance = sqrtf(x + y);
	// ���� �Ÿ� �̳����, Move State�� ����
	if (fabsf(distance) < 700.0f * WSCALEX) {
		actor.stateEnum_ = LittleGhost::LittleGhostStateEnum::MOVE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
	//actor.state_->enter(*this);	// å�� �ִ� ����	
}

void IdleStateLG::Action(LittleGhost& actor)
{
	// �ϴ°� ���µ�, ���������� �ʰ�...

	actor.GroundCheck();
	actor.GravityUpdate();
}

void IdleStateLG::Enter(LittleGhost& actor)
{
	actor.GetAnimation()->SetPlay(static_cast<int>(actor.stateEnum_));
	// ��Ʈ���� ����
}
