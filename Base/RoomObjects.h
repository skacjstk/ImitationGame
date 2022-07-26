#pragma once
// ������: �̱����� �ƴϸ�, �� Room ���� ��ü�� �����ϰ� �Ϸ��� ��. �� �ܿ� ����� ��� ����.
class RoomObjects
{
public:
	void		 Update(string name, Matrix V, Matrix P);
	void		 Render(string name);
	void		 UpdateAll(Matrix V, Matrix P);
	void		 RenderAll();

	void         SetPosition(string name, Vector2 position);
	void         SetPosition(string name, float x, float y);
	void		 AddObject(string name, GameObject* obj);
	GameObject* FindObject(string name);
	void         AddObjectStrings(string name) { m_cmObjectStrings.push_back(name); };
	void         ClearObjectStrings() { m_cmObjectStrings.clear(); }
	auto		 GetObjectsMap() { return &m_cmObjects; }
private:
	map<string, GameObject*> m_cmObjects;
	vector<string>  m_cmObjectStrings;

public:	// ���� �̱����� �ƴϴϻ����� �����ϰ�
	RoomObjects();
public:
	~RoomObjects();

};
