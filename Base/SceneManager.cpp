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
	currentScene = m_cvScenes[0];	//��ĭ�ε� �翬�� 0���� ���ְ���?

	//�����带 �̿��Ͽ� ���� ���� Loading�Ѵ�.
	printf("SceneManager ������ ȣ��\n");
	//WndProc --> �ݹ��Լ� ó�� 
	thread t(bind(&SceneManager::ThreadStart, this));
	t.detach(); //wait ���� ������ ����
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
	CAMERA->Update();	// ī�޶��� ��ġ�� ��ġ�� �� Update(V P) �� ī�޶��� Texture ��ǥ�� ������Ʈ Next
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
	playerUI->Render();	// UI �׽�Ʈ�ڵ�.
	Display();
}
/*
������ ���� ������Ʈ�� ���⼭
*/
void SceneManager::CreateGameObject()
{
	// ���� �̸� ����� ���� ��. ���߿� ������ Add �Ҷ�, ���⿡ �̹� �ִ� �Ŷ�� �ܼ��� Find�� ã�� �Ѱ��ֱ⸸ �� �� 
	// �ٵ� ���׸��屸���� ����鼭 �̰� ��ó�� �ȵǳ�
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
			currentScene = m_cvScenes[i];	//���� ����߽� Scene
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
/// ����: Thread �� Process�� ������ 
/// //	Code: �츮�� �ڵ�§�� 
//	Data: ��������, static ���� (���������� �ȴٴµ�? ��..) -> ������µ� ������� �������� stack��.
//	Heap : �����Ҵ� ����
//	Stack : �Ű�����, �Լ� ��������, �Լ� �����ּ�

// Next
void SceneManager::Display()
{
	
	// FPS Display
	DirectWrite::BeginDraw();
	{
		RECT rect = { 0,0,500,200 };	// Window ��ǥ
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
		CAMERA->WCtoVC(pos);	// View ��ǥ�� ��ȯ 
		str = L"Mouse :" + to_wstring(pos.x) + L", " + to_wstring(pos.y);
		DirectWrite::RenderText(str, rect);
	}
	DirectWrite::EndDraw();
}

void SceneManager::ThreadStart()
{
	printf("������ Start\n");
	m_cvScenes.push_back(new S04_Extra01());
	m_cvScenes.push_back(new S18_AStarDemo());
	m_cvScenes.push_back(new Town());
	m_cvScenes.push_back(new Floor_1());
	m_cvScenes.push_back(new Floor_2());
	printf("���� ������ ����: %zd\n", m_cvScenes.size());

	printf("������ End\n");
}
