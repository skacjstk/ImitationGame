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
	void Update(float lockFPS);		//�ʴ� ������ ����
	float Delta() { return timeElepased; }
	float GetRunningTime() { return runningTime; }
	float GetFPS() { return framePerSecond; }
	int GetFrame() { return frameCountPerSecond; }
private:
	std::chrono::system_clock::time_point cycleTime;	//start �ð� �� �����ֱ�
	float timeElepased = 0.0f;		//Update �� ��� �ð�
	float fpsTimeElepased = 0.0f;	//Frame
	float runningTime = 0.0f;		//�� running �ð�
	float framePerSecond = 0.0f;	//
	float frameCount = 0;
	int frameCountPerSecond = 144;	//�ʴ� Frame Count

	GameTime();
public:
	~GameTime();

};

