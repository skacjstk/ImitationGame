#include "ImitationGame/framework.h"
#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	for (auto a : m_cmObjects)
		SAFE_DELETE(a.second);
	m_cmObjects.clear();
	ClearObjectStrings();

}

void ObjectManager::Update(string name, Matrix V, Matrix P)
{
	GameObject* pObject = FindObject(name);

	if (!pObject) return;

	pObject->Update(V, P);

}
void ObjectManager::Render(string name)
{
	GameObject* pObject = FindObject(name);

	if (!pObject) return;

	pObject->Render();

}
void ObjectManager::UpdateAll(Matrix V, Matrix P)
{
	for (auto a : this->m_cmObjectStrings)
		Update(a, V, P);

}
void ObjectManager::RenderAll()
{
	for (auto a : this->m_cmObjectStrings)
	{
		if (a != "nodisplay")
			Render(a);
	}

}
void ObjectManager::SetPosition(string name, Vector2 position)
{
	GameObject* pObject = FindObject(name);

	if (!pObject) return;

	pObject->SetPosition(position);
}
void ObjectManager::SetPosition(string name, float x, float y)
{
	GameObject* pObject = FindObject(name);

	if (!pObject) return;

	pObject->SetPosition(x, y);
}
///////////////////////////////////////////////////////
// map�� object�� add
///////////////////////////////////////////////////////
void ObjectManager::AddObject(string name, GameObject* obj)
{
	// �����Ѱ��� 1���̻� �־ insert�ȵ�
	m_cmObjects.insert(make_pair(name, obj));
}
////////////////////////////////////////////////////////
// Obhect�� �̸��� ������ �ش� class return
///////////////////////////////////////////////////////
GameObject* ObjectManager::FindObject(string name)
{
	GameObject* pObject = nullptr;

	if (m_cmObjects.count(name))
		pObject = m_cmObjects[name];

	return pObject;
}


