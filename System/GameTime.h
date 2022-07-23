#pragma once
#define TIMEMANAGER GameTime::GetInstance()
class GameTime
{
public:
	static GameTime* GetInstance() {
		static GameTime singleTon;
		return &singleTon;
	}
public:
	void Update();
	void Update(float lockFPS);		//초당 프레임 제한
	float Delta() { return timeElepased; }
	float GetRunningTime() { return runningTime; }
	float GetFPS() { return framePerSecond; }
	int GetFrame() { return frameCountPerSecond; }
private:
	std::chrono::system_clock::time_point cycleTime;	//start 시간 및 갱신주기
	float timeElepased = 0.0f;		//Update 당 경과 시간
	float fpsTimeElepased = 0.0f;	//Frame
	float runningTime = 0.0f;		//총 running 시간
	float framePerSecond = 0.0f;	//
	float frameCount = 0;
	int frameCountPerSecond = 144;	//초당 Frame Count

	GameTime();
public:
	~GameTime();

};

