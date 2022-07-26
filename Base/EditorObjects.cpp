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
// map에 object를 add
///////////////////////////////////////////////////////
void EditorObjects::AddObject(string name, Texture* obj)
{
	// 동일한것을 1개이상 넣어도 insert안됨
	m_cmObjects.insert(make_pair(name, obj));
}
////////////////////////////////////////////////////////
// Object의 이름을 넣으면 해당 class return
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
	// 해당 오브젝트의 string, 해당 오브젝트의 위치(맵 오프셋 가져와야겠지?), 오브젝트 타입을 string 화 한것, 해당 텍스쳐의 경로(editor용)
}

void EditorObjects::LoadObjectsFile(string fileName)
{
	// 해당 오브젝트의 string, 해당 오브젝트의 위치(맵 오프셋 가져와야겠지?), 오브젝트 타입을 string 화 한것 한줄씩 불러오며 m_cmObjects에 넣기
}


