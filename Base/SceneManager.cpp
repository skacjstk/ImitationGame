#include "ImitationGame/framework.h"
#include "SceneManager.h"

// Include Scene
#include "S01_DXLine.h"

SceneManager::SceneManager()
{
//	CreateGameObject();
	m_cvScenes.push_back(new S01_DXLine());
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
//	CAMERA->Update();	// ī�޶��� ��ġ�� ��ġ�� �� Update(V P) �� ī�޶��� Texture ��ǥ�� ������Ʈ Next
	for (UINT i = 0; i < m_cvScenes.size(); ++i) {
		if (m_cvScenes[i]->IsActive()) {
			currentScene = m_cvScenes[i];	//���� ����߽� Scene
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
//	Display();
}
/*
������ ���� ������Ʈ�� ���⼭
*/
void SceneManager::CreateGameObject()
{
	// ���� �̸� ����� ���� ��. ���߿� ������ Add �Ҷ�, ���⿡ �̹� �ִ� �Ŷ�� �ܼ��� Find�� ã�� �Ѱ��ֱ⸸ �� �� 

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
/// ����: Thread �� Process�� ������ 
/// //	Code: �츮�� �ڵ�§�� 
//	Data: ��������, static ���� (���������� �ȴٴµ�? ��..) -> ������µ� ������� �������� stack��.
//	Heap : �����Ҵ� ����
//	Stack : �Ű�����, �Լ� ��������, �Լ� �����ּ�

// Next
void SceneManager::Display()
{
	/*
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
		
	if (SCENEMANAGER->GetCurrentScene()->IsDisplay())	// �� Scene���� Display���� ���̸� �̰� ������.
	{
		// HP Display�� HS01�� ���� ������ �õ���. 0526
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
// HP ǥ��
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

				Vector2 position = Vector2(-750.0f, -270.0f);	// Windows ��ǥ 
				position = position + Vector2(fontSize * -0.5f * str.length(), fontSize * +0.5f);
				CAMERA->VCtoWC(position);

				if (pObject->GetHP() >= 0)
					DirectWrite::RenderText(str, position, 255, 255, 255, fontSize);
				// Scene �� ��� ������
				wstring temp = L"��������������������00";
				int SceneID = SCENEMANAGER->GetCurrentScene()->GetSceneID() - 1;
				if (SceneID < 0) SceneID = 0;
				str = temp.at(SceneID);
				position = Vector2(750.0f, -270.0f);	// Windows ��ǥ 
				position = position + Vector2(fontSize * -0.5f * str.length(), fontSize * +0.5f);
				CAMERA->VCtoWC(position);
				if (pObject->GetHP() >= 0)	// HP �� 0 �̸��̸� DeadScene�̶�
					DirectWrite::RenderText(str, position, 255, 255, 255, fontSize);
			}
		}
		DirectWrite::EndDraw();
	}
	*/
}

void SceneManager::ThreadStart()
{
	printf("������ Start\n");
//	m_cvScenes.push_back(new S02_LoadingScene());
	printf("���� ������ ����: %zd\n", m_cvScenes.size());

	printf("������ End\n");
}
