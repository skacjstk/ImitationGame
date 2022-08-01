#pragma once

// Scene이 1~100까지 있다고 할때, Loading시 시간 걸리는 경우
// IntroScene 은 간단하게 만들어 Loading되는 모습 보여줄 예정
// 나머지 Scene들은 멀티쓰레드를 이용하여 백그라운드로 생성한다.
#define SCENEMANAGER SceneManager::GetInstance()
class SceneManager
{
public:
	static SceneManager* GetInstance() 
	{
		static SceneManager singleTon; // 생성이 두번되지는 않는 정적
		return &singleTon;
	}
	//단점: 포인터라서 해제해줘야되서 요즘은 스태틱으로 함.
//	static SceneManager* m_pInstance;
//	SceneManager* Get(){
//		return m_pInstance;
//	}
//	void Create(){
//		m_pInstance = new SceneManager();
//	}
	void Update();
	void Render();
	void CreateGameObject();
	void ChangeScene(string name);
	Scene* GetScene(string name);
	vector<string> sceneNames_ = {
		"Intro", "Town" , "Floor_1" , "Floor_2"
	};
public:		//Getter
	Scene* GetCurrentScene() { return currentScene; }
private:
	vector<class Scene*> m_cvScenes; //여러 Scene을 담아놓는 벡터 컨테이너
	Scene* currentScene = nullptr;

	SceneManager();
public:
	~SceneManager();
private:
	void Display();
	void ThreadStart();	//여러 Scene이 있는 경우 여기서 Scene 생성
};

