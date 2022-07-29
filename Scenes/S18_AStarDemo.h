#pragma once
////////////////////////////////////////////////
//  
//  Axis Alignment Boundary Box
//  
////////////////////////////////////////////////

class S18_AStarDemo : public Scene
{

public:
	void Update()  override;
	void Render()  override;
	void ChangeScene() override;
	void ExitScene()	override;
private:
	void  ClearWall();
	void  ClearPath();
	void  RunAStar();
	bool  ExistWall(Vector2 pos);
	class Texture       *m_pChocobo      = nullptr;
	vector<class Line*>  m_cvGrid;        // Grid Line
	vector<class Rect*>  m_cvWalls;       // Mouse�� �Էµ� Wall
	vector<class Rect*>  m_cvPaths;       // AStar --> ��� Path 
	class  Rect          *m_pGoal         = nullptr;  // Goal
	class  Rect          *m_pStart        = nullptr;  // Start

public:
	S18_AStarDemo();
	~S18_AStarDemo();
};