#include "ImitationGame/framework.h"
#include "Object/Enemy/LittleGhost.h"
#include "State/LittleGhostState/LittleGhostState.h"
#include "State/LittleGhostState/IdleStateLG.h"
#include "MoveStateLG.h"

void MoveStateLG::SwitchState(LittleGhost& actor)
{
	// ���� ��ġ ���� �� ��ġ�� �ﰢ�Լ� �Ÿ����� �⺻
	float x = actor.chaseTarget_->GetPosition().x - actor.GetPosition().x;
	float y = actor.chaseTarget_->GetPosition().y - actor.GetPosition().y;
	float x2 = pow(x, 2);
	float y2 = pow(y, 2);
	// 2������ ���� ���� ������ = �Ÿ�
	distance = sqrtf(x2 + y2);	
	radian = atan2(y, x);
	// �� ��������� Attack���� ����
	if (distance < actorScale * 60.0f) {
		actor.stateEnum_ = LittleGhost::LittleGhostStateEnum::ATTACK;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
	}
}

void MoveStateLG::Action(LittleGhost& actor)
{
	beforePos = actor.GetPosition();
	actor.Move(radian);

}

void MoveStateLG::Enter(LittleGhost& actor)
{
	actorScale = actor.GetScale().x;
	actor.GetAnimation()->SetPlay(static_cast<int>(actor.stateEnum_));
}
