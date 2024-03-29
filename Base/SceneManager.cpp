#include "ImitationGame/framework.h"
#include "SceneManager.h"

// Include Scene
#include "Scenes/S04_Extra01.h"
#include "Scenes/S18_AStarDemo.h"
#include "Scenes/Intro.h"
#include "Scenes/Floor_1.h"
#include "Scenes/Floor_2.h"
#include "Scenes/Town.h"

SceneManager::SceneManager()
{
	Audio->AddSoundAll();
	m_cvScenes.push_back(new Intro());
	currentScene = m_cvScenes[0];	//빈칸인데 당연히 0번에 들어가있겠지?

	//스레드를 이용하여 여러 씬을 Loading한다.
	printf("SceneManager 생성자 호출\n");
	//WndProc --> 콜백함수 처럼 
	thread t(bind(&SceneManager::ThreadStart, this));
	t.detach(); //wait 없이 쓰레드 동작
}

SceneManager::~SceneManager()
{
	for (UINT i = 0; i < m_cvScenes.size(); ++i) {
		delete m_cvScenes[i];
	}
	m_cvScenes.erase(m_cvScenes.begin(), m_cvScenes.end());
}


void SceneManager::Update()
{
	CAMERA->Update();	// 카메라의 위치를 고치는 것 Update(V P) 는 카메라의 Texture 좌표를 업데이트 Next
	for (UINT i = 0; i < m_cvScenes.size(); ++i) {
		if (m_cvScenes[i]->IsActive()) {
			m_cvScenes[i]->Update();
		}
	}
}

void SceneManager::Render()
{
	for (UINT i = 0; i < m_cvScenes.size(); ++i) {
		if (m_cvScenes[i]->IsActive()) {
			m_cvScenes[i]->Render();
		}
	}
	playerUI->Render();	// UI 테스트코드.
	CAMERA->Render();	// 카메라에 등록된 이펙트 출력
	Display();
}

void SceneManager::ChangeScene(string name)
{
	for (UINT i = 0; i < m_cvScenes.size(); ++i)
	{
		m_cvScenes[i]->SetActive(false);
		if (name == m_cvScenes[i]->GetName()) {
			m_cvScenes[i]->SetActive(true);
			currentScene = m_cvScenes[i];	//현재 재생중신 Scene
			m_cvScenes[i]->ChangeScene();
		}
	}
}
Scene * SceneManager::GetScene(string name)
{
	Scene* value = nullptr;
	for (UINT i = 0; i < m_cvScenes.size(); ++i)
	{
		if (name == m_cvScenes[i]->GetName()) {
			value = m_cvScenes[i];
			break;
		}
	}
	return value;
}
///
/// 숙제: Thread 와 Process의 차이점 
/// //	Code: 우리가 코드짠거 
//	Data: 전역변수, static 변수 (지역변수도 된다는데? 음..) -> 변수라는데 어딜봐도 지역변수 stack임.
//	Heap : 동적할당 영역
//	Stack : 매개변수, 함수 지역변수, 함수 리턴주소

// Next
void SceneManager::Display()
{
	
	// FPS Display
	DirectWrite::BeginDraw();
	{
		RECT rect = { 0,0,500,200 };	// Window 좌표
		wstring str = L"FPS : " + to_wstring(TIMEMANAGER->GetFrame());
		DirectWrite::RenderText(str, rect);

		rect.top += 20;
		rect.bottom += 20;
		str = L"Camera :" + to_wstring(CAMERA->GetPosition().x)
			+ L", " + to_wstring(CAMERA->GetPosition().y);
		DirectWrite::RenderText(str, rect);

		rect.top += 20;
		rect.bottom += 20;
		Vector2 pos = Mouse->GetPosition();
		CAMERA->WCtoVC(pos);	// View 좌표로 변환 
		str = L"Mouse :" + to_wstring(pos.x) + L", " + to_wstring(pos.y);
		DirectWrite::RenderText(str, rect);
	}
	DirectWrite::EndDraw();
}

void SceneManager::ThreadStart()
{
	printf("쓰레드 Start\n");
	m_cvScenes.push_back(new S04_Extra01());
	m_cvScenes.push_back(new S18_AStarDemo());
	m_cvScenes.push_back(new Town());
	m_cvScenes.push_back(new Floor_1());
	m_cvScenes.push_back(new Floor_2());
	printf("현재 쓰레드 개수: %zd\n", m_cvScenes.size());

	printf("쓰레드 End\n");
}
