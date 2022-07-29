#include "ImitationGame/framework.h"
#include "AStar.h"
#include <vector>
#include <functional>
#include <set>
#include <algorithm>

#define MAX(x,y) ( ( ((x) >= (y)) ? (x):(y)) )
#define MIN(x,y) ( ( ((x) <  (y)) ? (x):(y)) )
#define ABS(x) ( ( ((x) >  (0)) ? (x):(-(x))) )

////////////////////////////////////////////////////////
// 생성자	https://itmining.tistory.com/66
///////////////////////////////////////////////////////
AStar::AStar()
{
	SetDiagonalMovement(true);
	//setHeuristic(&Heuristic::manhattan);
	direction = 
	{
		{ 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 },
		{ -1, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }
	};
}

AStar::~AStar()
{
}
vector<PointDB> AStar::FindPath(int sx, int sy, int ex, int ey)
{
	PointDB source = PointDB(sx, sy);
	PointDB target = PointDB(ex, ey);

	return FindPath(source, target);
}
vector<PointDB> AStar::FindPath(PointDB source, PointDB target)
{
	Node         *current = nullptr;
	vector<Node*> openSet, closedSet;
	openSet.reserve(100);        
	closedSet.reserve(100);
	openSet.push_back(new Node(source));

	while (!openSet.empty())
	{
		auto current_it = openSet.begin();
		current = *current_it;

		for (auto it = openSet.begin(); it != openSet.end(); it++)
		{
			auto node = *it;
			if (node->GetScore() <= current->GetScore()) {
				current = node;
				current_it = it;
			}
		}

		if (current->coordinates == target) 
		{
			break;
		}

		closedSet.push_back(current);
		openSet.erase(current_it);

		for (unsigned int i = 0; i < directions; ++i)
		{
			//PointDB newCoordinates(current->coordinates + direction[i]);
			PointDB newCoordinates;
			newCoordinates.x = current->coordinates.x + direction[i].x;
			newCoordinates.y = current->coordinates.y + direction[i].y;
			if (DetectCollision(newCoordinates) ||
				FindNodeOnList(closedSet, newCoordinates)) 
			{
				continue;
			}

			unsigned int totalCost = current->G + ((i < 4) ? 10 : 14);

			Node *successor = FindNodeOnList(openSet, newCoordinates);
			if (successor == nullptr) 
			{
				successor = new Node(newCoordinates, current);
				successor->G = totalCost;
				successor->H = Heuristic(successor->coordinates, target);
				openSet.push_back(successor);
			}
			else if (totalCost < successor->G) {
				successor->parent = current;
				successor->G = totalCost;
			}
		}
	}

	vector<PointDB> path;
	while (current != nullptr)
	{
		path.push_back(current->coordinates);
		current = current->parent;
	}

	ReleaseNodes(openSet);
	ReleaseNodes(closedSet);

	return path;
}
void AStar::SetDiagonalMovement(bool enable)
{
	directions = (enable ? 8 : 4);
}
void AStar::SetWorldSize(int x, int y)
{
	worldSize.x = x;
	worldSize.y = y;
}
////////////////////////////////////////////////////////
// Wall 관련 함수
////////////////////////////////////////////////////////
void AStar::AddCollision(PointDB pt)
{
	walls.push_back(pt);
}
void AStar::AddCollision(int x, int y)
{
		AddCollision(PointDB(x, y));
}
PointDB AStar::GetDelta(PointDB source, PointDB target)
{
	return{ abs(source.x - target.x),  abs(source.y - target.y) };
}
unsigned int AStar::Heuristic(PointDB source, PointDB target)
{
	unsigned int Value;
	switch (m_nHeuristic)
	{
	case 0:
		Value = Euclidean(source, target);
		break;
	case 1:
		Value = Manhattan(source, target);   // 사선 X
		break;
	case 2:
		Value = Octagonal(source, target);    // 사선
		break;
	}
	return Value;
}
unsigned int AStar::Manhattan(PointDB source, PointDB target)
{
	auto delta = std::move(GetDelta(source, target));
	return static_cast<unsigned int>(10 * (delta.x + delta.y));
}
unsigned int AStar::Euclidean(PointDB source, PointDB target)
{
	auto delta = std::move(GetDelta(source, target));
	return static_cast<unsigned int>(10 * sqrt(pow(delta.x, 2) + pow(delta.y, 2)));
}
unsigned int AStar::Octagonal(PointDB source, PointDB target)
{
	auto delta = std::move(GetDelta(source, target));
	return 10 * (delta.x + delta.y) + (-6) * MIN(delta.x, delta.y);
}
void AStar::RemoveCollision(PointDB pt)
{
	auto it = std::find(walls.begin(), walls.end(), pt);
	if (it != walls.end()) 
	{
		walls.erase(it);
	}
}
void AStar::ClearCollisions()
{
	walls.clear();
}

bool AStar::DetectCollision(PointDB pt)
{
	if (pt.x < 0 || pt.x >= worldSize.x ||
		pt.y < 0 || pt.y >= worldSize.y ||
		std::find(walls.begin(), walls.end(), pt) != walls.end()) 
	{
		return true;
	}
	return false;
}

void AStar::ReleaseNodes(vector<Node*>& nodes_)
{
	for (auto it = nodes_.begin(); it != nodes_.end();) 
	{
		delete *it;
		it = nodes_.erase(it);
	}
}

Node * AStar::FindNodeOnList(vector<Node*>& nodes_, PointDB coordinates_)
{
	for (auto node : nodes_)
	{
		if (node->coordinates == coordinates_)
		{
			return node;
		}
	}
	return nullptr;
}



