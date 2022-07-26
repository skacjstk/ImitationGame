#pragma once
// ������: �̱����� �ƴϸ�, Editor �������� Texture�� ��µǰ� �ϴ� object��� txt�� RoomObjects�� ���� ���� ����� ����. 
class EditorObjects
{
public:
	void		 Update(string name, Matrix V, Matrix P);
	void		 Render(string name);
	void		 UpdateAll(Matrix V, Matrix P);
	void		 RenderAll();

	void         SetPosition(string name, Vector2 position);
	void         SetPosition(string name, float x, float y);
	void		 AddObject(string name, Texture* obj);
	Texture*	 FindObject(string name);
	void         AddObjectStrings(string name) { m_cmObjectStrings.push_back(name); };
	void         ClearObjectStrings() { m_cmObjectStrings.clear(); }
	auto		 GetObjectsMap() { return &m_cmObjects; }
	void		 SaveObjectsFile(string fileName);
	void		 LoadObjectsFile(string fileName);	// TRNMANAGER->setSceneMap ����
private:
	map<string, Texture*> m_cmObjects;
	vector<string>  m_cmObjectStrings;
	vector<string>  m_cmObjectTypes;

public:	// ���� �̱����� �ƴϴϻ����� �����ϰ�
	EditorObjects();
public:
	~EditorObjects();

};
