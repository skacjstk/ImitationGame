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
	std::chrono::duration<double> sec;	//현재 시간을 받은 다음에

	//현재 시간과 이전 주기 시간과의 duration (흐른 시간의 간격)
	sec = std::chrono::system_clock::now() - cycleTime;
	//한 Update당 시간	--> Delta()를 통해 각 Object 별로 시간측정을 할 것
	timeElepased = (float)sec.count();

	//여기서 cycleTime에 다시 값을 넣고, 다음 Update() 호출 때 빼지는 "기준"이 될 것
	cycleTime = std::chrono::system_clock::now();	//주기 기준에 현재시간 복사 
	++frameCount;

	// Game에서 동작한 시간을 저장
	runningTime += timeElepased;	//지금까지의 경과 시간을 기록
	fpsTimeElepased += timeElepased;	//몇 프레임이 1초지?

	if (fpsTimeElepased > 1.0f) //초당 프레임 계산
	{
		framePerSecond = fpsTimeElepased / frameCount;
		frameCountPerSecond = (int)frameCount;
		frameCount = fpsTimeElepased = 0.0f;
	}//end if

//	printf("%f	%d\n", timeElepased, frameCountPerSecond);

}
void GameTime::Update(float lockFPS)
{
	std::chrono::duration<double> sec;	//현재 시간을 받은 다음에

//현재 시간과 이전 주기 시간과의 duration (흐른 시간의 간격)
	sec = std::chrono::system_clock::now() - cycleTime;
	//한 Update당 시간	--> Delta()를 통해 각 Object 별로 시간측정을 할 것
	timeElepased = (float)sec.count();
//	printf("%f \n", timeElepased);	// 현재시간 - 직전 프레임시간 = update,render 걸린 시간

	//주기가 될 때까지 여길 벗어나지 못함
	while (timeElepased < 1.0f / lockFPS) {
		sec = std::chrono::system_clock::now() - cycleTime;
		timeElepased = (float)sec.count();
	}
	//여기서 cycleTime에 다시 값을 넣고, 다음 Update() 호출 때 빼지는 "기준"이 될 것
	cycleTime = std::chrono::system_clock::now();	//주기 기준에 현재시간 복사 
	++frameCount;

	// Game에서 동작한 시간을 저장
	runningTime += timeElepased;	//지금까지의 경과 시간을 기록
	fpsTimeElepased += timeElepased;	//몇 프레임이 1초지?

	if (fpsTimeElepased > 1.0f) //초당 프레임 계산
	{
		framePerSecond = fpsTimeElepased / frameCount;
		frameCountPerSecond = (int)frameCount;
		frameCount = fpsTimeElepased = 0.0f;
	}//end if

//	printf("lockUpdate()%f	%d\n", timeElepased, frameCountPerSecond);

}


