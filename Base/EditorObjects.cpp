#include "ImitationGame/framework.h"
#include "EditorObjects.h"

EditorObjects::EditorObjects()
{
}

EditorObjects::~EditorObjects()
{
	for (auto a : m_cmObjects)
		SAFE_DELETE(a.second);
	m_cmObjects.clear();
	ClearObjectStrings();

}

void EditorObjects::Update(string name, Matrix V, Matrix P)
{
	Texture* pObject = FindObject(name);

	if (!pObject) return;

	pObject->Update(V, P);

}
void EditorObjects::Render(string name)
{
	Texture* pObject = FindObject(name);

	if (!pObject) return;

	pObject->Render();

}
void EditorObjects::UpdateAll(Matrix V, Matrix P)
{
	for (auto a : this->m_cmObjectStrings)
		Update(a, V, P);

}
void EditorObjects::RenderAll()
{
	for (auto a : this->m_cmObjectStrings)
	{
		if (a != "nodisplay")
			Render(a);
	}

}
void EditorObjects::SetPosition(string name, Vector2 position)
{
	Texture* pObject = FindObject(name);

	if (!pObject) return;

	pObject->SetPosition(position);
}
void EditorObjects::SetPosition(string name, float x, float y)
{
	Texture* pObject = FindObject(name);

	if (!pObject) return;

	pObject->SetPosition(x, y);
}
///////////////////////////////////////////////////////
// map�� object�� add
///////////////////////////////////////////////////////
void EditorObjects::AddObject(string name, Texture* obj)
{
	// �����Ѱ��� 1���̻� �־ insert�ȵ�
	m_cmObjects.insert(make_pair(name, obj));
}
////////////////////////////////////////////////////////
// Object�� �̸��� ������ �ش� class return
///////////////////////////////////////////////////////
Texture* EditorObjects::FindObject(string name)
{
	Texture* pObject = nullptr;

	if (m_cmObjects.count(name))
		pObject = m_cmObjects[name];

	return pObject;
}

void EditorObjects::SaveObjectsFile(string fileName)
{
	// �ش� ������Ʈ�� string, �ش� ������Ʈ�� ��ġ(�� ������ �����;߰���?), ������Ʈ Ÿ���� string ȭ �Ѱ�, �ش� �ؽ����� ���(editor��)
}

void EditorObjects::LoadObjectsFile(string fileName)
{
	// �ش� ������Ʈ�� string, �ش� ������Ʈ�� ��ġ(�� ������ �����;߰���?), ������Ʈ Ÿ���� string ȭ �Ѱ� ���پ� �ҷ����� m_cmObjects�� �ֱ�
}


