#pragma once

// Scene�� 1~100���� �ִٰ� �Ҷ�, Loading�� �ð� �ɸ��� ���
// IntroScene �� �����ϰ� ����� Loading�Ǵ� ��� ������ ����
// ������ Scene���� ��Ƽ�����带 �̿��Ͽ� ��׶���� �����Ѵ�.
#define SCENEMANAGER SceneManager::GetInstance()
class SceneManager
{
public:
	static SceneManager* GetInstance() 
	{
		static SceneManager singleTon; // ������ �ι������� �ʴ� ����
		return &singleTon;
	}
	//����: �����Ͷ� ��������ߵǼ� ������ ����ƽ���� ��.
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
	vector<class Scene*> m_cvScenes; //���� Scene�� ��Ƴ��� ���� �����̳�
	Scene* currentScene = nullptr;

	SceneManager();
public:
	~SceneManager();
private:
	void Display();
	void ThreadStart();	//���� Scene�� �ִ� ��� ���⼭ Scene ����
};

