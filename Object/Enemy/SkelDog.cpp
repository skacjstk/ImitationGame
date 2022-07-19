#include "ImitationGame/framework.h"
#include "State/State.h"
#include "State/SkelDogState.h"
#include "State/IdleState.h"
#include "State/RunState.h"
#include "SkelDog.h"

SkelDog::SkelDog()
{
	state_[0] = new IdleState();
	state_[1] = new RunState();
	currentState_ = state_[0];
	
}

SkelDog::~SkelDog()
{
}

void SkelDog::Update(Matrix V, Matrix P)
{
	// IDLE, RUN, ATTACK(RUN 0 �� �������� �پ������)
	StateUpdate();


}

void SkelDog::Render()
{
}

void SkelDog::Reset()
{
}

void SkelDog::StateUpdate()
{
	currentState_->SwitchState(*this);
	currentState_->Action(*this);
}

void SkelDog::LeftMove()
{
}

void SkelDog::RightMove()
{
}

void SkelDog::Jump()
{
}

void SkelDog::Move()
{
	printf("�ذ� Move\n");
}

void SkelDog::Idle()
{
	printf("�ذ� Idle\n");
}

void SkelDog::Attack()
{
}
