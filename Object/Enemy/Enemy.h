#pragma once
class Enemy : public GameActor
{
public: // ������ & �Ҹ���
	Enemy();
	~Enemy();
public:	// ���� �ν��Ͻ� ���� 
	virtual void Update(Matrix V, Matrix P) = 0;  // ���������Լ�
	virtual void Render() = 0;                    // ���������Լ�
	virtual void Reset() = 0;                    // ���������Լ�
};
