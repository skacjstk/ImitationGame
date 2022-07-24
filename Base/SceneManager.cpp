#include "ImitationGame/framework.h"
#include "SceneManager.h"

// Include Scene
#include "Scenes/S04_Extra01.h"
#include "Scenes/Intro.h"
#include "Scenes/Floor_1.h"
#include "Scenes/Town.h"

SceneManager::SceneManager()
{
	m_cvScenes.push_back(new Intro());
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
			currentScene = m_cvScenes[i];	//현재 재생중신 Scene
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
	Display();
}
/*
앞으로 만들 오브젝트는 여기서
*/
void SceneManager::CreateGameObject()
{
	// 여긴 미리 만들어 놓는 것. 나중에 씬에서 Add 할때, 여기에 이미 있는 거라면 단순히 Find로 찾아 넘겨주기만 할 뿐 

	// Box Create
//	for (int i = 0; i < 10; ++i) {
//		string name = "Box" + to_string(i);
//		Box* pObject = (Box*)OBJECTMANAGER->FindObject(name);
//		if (!pObject) {
//			pObject = new Box();
//			OBJECTMANAGER->AddObject(name, pObject);
//		}
//	}

}

void SceneManager::ChangeScene(string name)
{
	for (UINT i = 0; i < m_cvScenes.size(); ++i)
	{
		m_cvScenes[i]->SetActive(false);
		if (name == m_cvScenes[i]->GetName()) {
			m_cvScenes[i]->SetActive(true);
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
	/*
	if (SCENEMANAGER->GetCurrentScene()->IsDisplay())	// 각 Scene에서 Display값이 참이면 이거 보여줌.
	{
		// HP Display를 HS01에 따로 빼려고 시도중. 0526
		// DisPlayHPandSceneID();
		// Map Display
		DirectWrite::BeginDraw();
		{
			for (int y = 0; y < HELLTAKERMAP->GetSizeY(); y++)
			{
				for (int x = 0; x < HELLTAKERMAP->GetSizeX(); x++)
				{
					Vector2 position = HELLTAKERMAP->GetPosition(x, y);
					int     state = HELLTAKERMAP->GetValue(x, y);
					CAMERA->VCtoWC(position);
					wstring  str = to_wstring(state);
					if (state == 0)
						DirectWrite::RenderText(str, position, 255, 0, 255, 20.0f);
					else
						DirectWrite::RenderText(str, position, 255, 255, 255, 20.0f);

					position.y = position.y - 1;
					str = L"(" + to_wstring(x) + L"," + to_wstring(y) + L")";
					//   if (state != 0)
					DirectWrite::RenderText(str, position, 0, 255, 255, 16.0f);
				}
			}
		}
		DirectWrite::EndDraw();
	}//end if(SceneID >= 0)
	*/
}
// Next 
// HP 표시
void SceneManager::DisPlayHPandSceneID()
{
	/*
	int SceneID = SCENEMANAGER->GetCurrentScene()->GetSceneID() - 1;
	if (SceneID >= 0) {
		DirectWrite::BeginDraw();
		{

			Helltaker* pObject = (Helltaker*)OBJECTMANAGER->FindObject("Helltaker");
			float fontSize = 180.0f;
			if (pObject) {
				wstring str = to_wstring(pObject->GetHP());
				if (pObject->GetHP() <= 0)
					str = L"X";

				Vector2 position = Vector2(-750.0f, -270.0f);	// Windows 좌표 
				position = position + Vector2(fontSize * -0.5f * str.length(), fontSize * +0.5f);
				CAMERA->VCtoWC(position);

				if (pObject->GetHP() >= 0)
					DirectWrite::RenderText(str, position, 255, 255, 255, fontSize);
				// Scene 명 출력 오른쪽
				wstring temp = L"ⅠⅡⅢⅣⅤⅥⅦⅧⅨⅩ00";
				int SceneID = SCENEMANAGER->GetCurrentScene()->GetSceneID() - 1;
				if (SceneID < 0) SceneID = 0;
				str = temp.at(SceneID);
				position = Vector2(750.0f, -270.0f);	// Windows 좌표 
				position = position + Vector2(fontSize * -0.5f * str.length(), fontSize * +0.5f);
				CAMERA->VCtoWC(position);
				if (pObject->GetHP() >= 0)	// HP 가 0 미만이면 DeadScene이라서
					DirectWrite::RenderText(str, position, 255, 255, 255, fontSize);
			}
		}
		DirectWrite::EndDraw();
	}
	*/
}

void SceneManager::ThreadStart()
{
	printf("쓰레드 Start\n");
	m_cvScenes.push_back(new Town());
	m_cvScenes.push_back(new S04_Extra01());
	m_cvScenes.push_back(new Floor_1());
	printf("현재 쓰레드 개수: %zd\n", m_cvScenes.size());

	printf("쓰레드 End\n");
}
