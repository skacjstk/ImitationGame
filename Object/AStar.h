#pragma once
class PointDB
{
public:
	int x = 0;
	int	y = 0;
	bool   operator == (const PointDB& coordinates_)
	{
		return (x == coordinates_.x && y == coordinates_.y);
	};
	PointDB operator + (const PointDB& coordinates_)
	{
		PointDB result;
		result.x = this->x + coordinates_.x;
		result.y = this->y + coordinates_.y;
		return result;
	}
public:
	PointDB() {};
	PointDB(int x_, int y_) { x = x_; y = y_; }
};

class Node
{
public:
	unsigned int G = 0;
	unsigned int H = 0;
	PointDB      coordinates;
	Node        *parent;
	unsigned    int GetScore()
	{
		return G + H;
	};
public:
	Node(PointDB coord_, Node *parent_ = nullptr)
	{
		parent = parent_;
		coordinates = coord_;
		G = H = 0;
	};
	~Node() {};
};


class AStar
{

public:
	vector<PointDB> FindPath(int sx, int sy, int ex, int ey);
	vector<PointDB> FindPath(PointDB source, PointDB target);

	// Setter
	void  SetDiagonalMovement(bool enable);
	void  SetWorldSize(int x, int y);
	void  SetHeuristic(int Type) { m_nHeuristic = Type; };

	// Wall
	void  AddCollision(PointDB    pt);
	void  AddCollision(int x, int y);;

private:
	// Heuristic
	PointDB       GetDelta(PointDB  source, PointDB target);
	unsigned int  Heuristic(PointDB source, PointDB target);
	unsigned int  Manhattan(PointDB source, PointDB target);  // 1
	unsigned int  Euclidean(PointDB source, PointDB target);  // 0
	unsigned int  Octagonal(PointDB source, PointDB target);  // 2

	// Wall
	void  RemoveCollision(PointDB pt);
	void  ClearCollisions();
	bool  DetectCollision(PointDB pt);

	// Astar
	void  ReleaseNodes(vector<Node*>& nodes_);
	Node* FindNodeOnList(vector<Node*>& nodes_, PointDB coordinates_);
private:
	int   m_nHeuristic = 0;  // 3°¡Áö Type
	vector<PointDB> direction;
	vector<PointDB> walls;
	PointDB   worldSize;
	unsigned int directions;

public:
	 AStar();
	~AStar();
};

