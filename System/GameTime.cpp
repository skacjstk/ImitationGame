#include "ImitationGame/framework.h"
#include "GameTime.h"

GameTime::GameTime()
{
	cycleTime = std::chrono::system_clock::now();
}

GameTime::~GameTime()
{
}
void GameTime::Update()
{
	std::chrono::duration<double> sec;	//���� �ð��� ���� ������

	//���� �ð��� ���� �ֱ� �ð����� duration (�帥 �ð��� ����)
	sec = std::chrono::system_clock::now() - cycleTime;
	//�� Update�� �ð�	--> Delta()�� ���� �� Object ���� �ð������� �� ��
	timeElepased = (float)sec.count();

	//���⼭ cycleTime�� �ٽ� ���� �ְ�, ���� Update() ȣ�� �� ������ "����"�� �� ��
	cycleTime = std::chrono::system_clock::now();	//�ֱ� ���ؿ� ����ð� ���� 
	++frameCount;

	// Game���� ������ �ð��� ����
	runningTime += timeElepased;	//���ݱ����� ��� �ð��� ���
	fpsTimeElepased += timeElepased;	//�� �������� 1����?

	if (fpsTimeElepased > 1.0f) //�ʴ� ������ ���
	{
		framePerSecond = fpsTimeElepased / frameCount;
		frameCountPerSecond = (int)frameCount;
		frameCount = fpsTimeElepased = 0.0f;
	}//end if

//	printf("%f	%d\n", timeElepased, frameCountPerSecond);

}
void GameTime::Update(float lockFPS)
{
	std::chrono::duration<double> sec;	//���� �ð��� ���� ������

//���� �ð��� ���� �ֱ� �ð����� duration (�帥 �ð��� ����)
	sec = std::chrono::system_clock::now() - cycleTime;
	//�� Update�� �ð�	--> Delta()�� ���� �� Object ���� �ð������� �� ��
	timeElepased = (float)sec.count();
//	printf("%f \n", timeElepased);	// ����ð� - ���� �����ӽð� = update,render �ɸ� �ð�

	//�ֱⰡ �� ������ ���� ����� ����
	while (timeElepased < 1.0f / lockFPS) {
		sec = std::chrono::system_clock::now() - cycleTime;
		timeElepased = (float)sec.count();
	}
	//���⼭ cycleTime�� �ٽ� ���� �ְ�, ���� Update() ȣ�� �� ������ "����"�� �� ��
	cycleTime = std::chrono::system_clock::now();	//�ֱ� ���ؿ� ����ð� ���� 
	++frameCount;

	// Game���� ������ �ð��� ����
	runningTime += timeElepased;	//���ݱ����� ��� �ð��� ���
	fpsTimeElepased += timeElepased;	//�� �������� 1����?

	if (fpsTimeElepased > 1.0f) //�ʴ� ������ ���
	{
		framePerSecond = fpsTimeElepased / frameCount;
		frameCountPerSecond = (int)frameCount;
		frameCount = fpsTimeElepased = 0.0f;
	}//end if

//	printf("lockUpdate()%f	%d\n", timeElepased, frameCountPerSecond);

}


