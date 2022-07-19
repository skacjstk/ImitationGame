#pragma once
class SkelDog : public GameActor
{
public:
	class SkelDogState* state_ = nullptr;
private:
	Animation* _animation = nullptr;
public: // ������ & �Ҹ���
	SkelDog();
	~SkelDog();
public:	// ���� �ν��Ͻ� ���� 
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;
	void  Reset() override;
	void StateUpdate();

	void LeftMove() override;
	void RightMove() override;
	void Jump() override;
	void Move() override;		// �Ļ���: 0630 ���� Left,Right���� ������ �������� Move�� ȣ���ϴ� ����. 
	void Idle() override;		// IDLE�� Attack�� �� �־�� ��.
	void Attack() override;

};
