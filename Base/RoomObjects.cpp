#include "ImitationGame/framework.h"
#include "RoomObjects.h"

RoomObjects::RoomObjects()
{
}

RoomObjects::~RoomObjects()
{
	for (auto a : m_cmObjects)
		SAFE_DELETE(a.second);
	m_cmObjects.clear();
	ClearObjectStrings();

}

void RoomObjects::Update(string name, Matrix V, Matrix P)
{
	GameObject* pObject = FindObject(name);

	if (!pObject) return;

	pObject->Update(V, P);

}
void RoomObjects::Render(string name)
{
	GameObject* pObject = FindObject(name);

	if (!pObject) return;

	pObject->Render();

}
void RoomObjects::UpdateAll(Matrix V, Matrix P)
{
	for (auto a : this->m_cmObjectStrings)
		Update(a, V, P);

}
void RoomObjects::RenderAll()
{
	for (auto a : this->m_cmObjectStrings)
	{
		if (a != "nodisplay")
			Render(a);
	}

}
void RoomObjects::SetPosition(string name, Vector2 position)
{
	GameObject* pObject = FindObject(name);

	if (!pObject) return;

	pObject->SetPosition(position);
}
void RoomObjects::SetPosition(string name, float x, float y)
{
	GameObject* pObject = FindObject(name);

	if (!pObject) return;

	pObject->SetPosition(x, y);
}
///////////////////////////////////////////////////////
// map�� object�� add
///////////////////////////////////////////////////////
void RoomObjects::AddObject(string name, GameObject* obj)
{
	// �����Ѱ��� 1���̻� �־ insert�ȵ�
	m_cmObjects.insert(make_pair(name, obj));
}
////////////////////////////////////////////////////////
// Object�� �̸��� ������ �ش� class return
///////////////////////////////////////////////////////
GameObject* RoomObjects::FindObject(string name)
{
	GameObject* pObject = nullptr;

	if (m_cmObjects.count(name))
		pObject = m_cmObjects[name];

	return pObject;
}


