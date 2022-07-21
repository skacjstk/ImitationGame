#include "ImitationGame/framework.h"
#include "Object/Enemy/SkelDog.h"
#include "State/SkelDogState/SkelDogState.h"
#include "State/SkelDogState/IdleState.h"
#include "AttackState.h"

void AttackState::SwitchState(SkelDog& actor)
{
	// y���� ó������ �������� ������ ���� ��.
	// ���̰ų�, �ٸ� ���� �浹�ߴٸ� attack�� ��� �ߴ��ϰ� IDLE ��
	//actor.IsGround() || actor.IsConflicted() || 
	if (time_ > 1.0f || firstAttackPos.y > actor.GetPosition().y) {
		actor.stateEnum_ = SkelDog::DogState::IDLE;
		actor.currentState_ = actor.state_[static_cast<int>(actor.stateEnum_)];
		actor.currentState_->Enter(actor);
		actor.moveSpeed = fabsf(actor.moveSpeed);	// Enter ���� ���� �ٲ�µ�, �װ� ����ȭ �ϴ� �ڵ�.
		time_ = 0.0f;
	}
}

void AttackState::Action(SkelDog& actor)
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

void AttackState::Enter(SkelDog& actor)
{
	time_ = 0.0f;
	dX = actor.chaseTarget_->GetPosition().x - actor.GetPosition().x;
	firstAttackPos = actor.GetPosition();
}
