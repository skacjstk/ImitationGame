#include "ImitationGame/framework.h"
#include "Object/Enemy/BigWhiteSkel.h"
#include "State/BigWhiteSkelState/BigWhiteSkelState.h"
#include "State/BigWhiteSkelState/IdleStateBWS.h"
#include "AttackStateBWS.h"

void AttackStateBWS::SwitchState(BigWhiteSkel& actor)
{
	// ������ �Ϸ��ߴٸ�, Next: 
	if (time_ > 1.0f) {
		actor.stateEnum_ = BigWhiteSkel::SkelState::IDLE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
		actor.moveSpeed = fabsf(actor.moveSpeed);	// Enter ���� ���� �ٲ�µ�, �װ� ����ȭ �ϴ� �ڵ�.
		time_ = 0.0f;
	}
}

void AttackStateBWS::Action(BigWhiteSkel& actor)
{
	//��ӵ� � ���� ����: �ٸ��� bakFall �˵��� �̻�����.
	actor.Attack(dX);
	time_ += TIMEMANAGER->Delta();
	/*	
	//�Ƹ� WCtoVC���� inverseMatrix ��� �ߴ��ź���... �װ� ���� y���� �������� �� �� �ִ�.
	//�Ŀ� V0�� �߸� ������. ���Ϳ����� ó���ӵ��ϻ� ��ǥ�� �ǹ��ϴ� �� �ƴϴ�.
	tempPosition.x = firstAttackPos.x + (actor.moveSpeed * time_);
	tempPosition.y = firstAttackPos.y + (actor.jumpSpeed * time_) + (-0.5f * G * 150.0f * time_ * time_);
	*/
}

void AttackStateBWS::Enter(BigWhiteSkel& actor)
{
	time_ = 0.0f;
	dX = actor.chaseTarget_->GetPosition().x - actor.GetPosition().x;
	firstAttackPos = actor.GetPosition();
}
