#pragma once
#define OBJECTMANAGER ObjectManager::GetInstance()

class ObjectManager
{
public:
	static  ObjectManager* GetInstance()
	{

		static ObjectManager instance;
		return &instance;
	}


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
private:
	map<string, GameObject*> m_cmObjects;
	vector<string>  m_cmObjectStrings;

private:
	ObjectManager();
public:
	~ObjectManager();

};
