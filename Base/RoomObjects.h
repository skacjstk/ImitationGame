#pragma once
// 차이점: 싱글턴이 아니며, 각 Room 마다 객체를 소유하게 하려고 함. 그 외에 기능은 모두 같음.
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

public:	// 이제 싱글턴이 아니니생성자 접근하게
	RoomObjects();
public:
	~RoomObjects();

};
