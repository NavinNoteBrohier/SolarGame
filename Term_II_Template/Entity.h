#pragma once
#include <iostream>
#include "string.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "Game Setup\main.h"
#include "Game Setup\sdlWindow.h"
#include "Game Setup\sdlResources.h"
#include "Game Setup\Helper.h"
#include "Screens\SpriteGame\ship.h"
#include "Screens\SpriteGame\SpriteGameRoutines.h"
#include <vector>
#include <algorithm>


class Entity
{
public:

	Entity();
	void Construct(vector<Node*>* a_nodes, vector<ship*>* a_ship, vector<int*>* a_idle);

	void updateEntity(vector <int*>* IdleShips);
	
	float rMetal;
	float rFuel;
	float FuelUse;
	int MetalThreshold;
	float turn;
	float CalculateFuelUse(vector <int*>* IdleShips);
	void CalculatePriotity(vector <int*>* IdleShips);
	void CollectIdleShips( vector <int*>* IdleShips, vector <ship*>* a_Ships);
	void CollectNodes();
	void FindRec();

	enum PriorityQ
	{
		pr_Metal, pr_Fuel
	};

private:
	vector<Node*>* m_nodes;
	vector<ship*>* m_ship;

	vector<int*> m_MetalNodes;
	vector<int*> m_FuelNodes;

	float m_x;
	float m_y;

	int C_Fuel;
	int C_Metal;

	int FuelThreshold;
	float FuelEconomy;

	int BuildCost;

	bool Alive;
	int m_Home;

	int m_cost;
	int TotalShips;
};

class Pathing : public Entity
{
public:
	
	static vector<Node*> FindPath(Node* start, Node* end, ship * a_ship);
	static bool CompareG(Node* a, Node* b);
	static bool FindNode(vector<Node*> Vector, Node* node);

private:

};