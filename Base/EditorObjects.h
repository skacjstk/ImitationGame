#pragma once
// 차이점: 싱글턴이 아니며, Editor 전용으로 Texture만 출력되게 하는 object들과 txt에 RoomObjects에 없는 저장 기능이 있음. 
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
	void		 LoadObjectsFile(string fileName);	// TRNMANAGER->setSceneMap 참고
private:
	map<string, Texture*> m_cmObjects;
	vector<string>  m_cmObjectStrings;
	vector<string>  m_cmObjectTypes;

public:	// 이제 싱글턴이 아니니생성자 접근하게
	EditorObjects();
public:
	~EditorObjects();

};
