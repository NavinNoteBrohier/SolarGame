#pragma once
#include "SDL.h"
#include <fstream> // input from a file
#include <string>
#include <vector>
#include "../../Game Setup/Helper.h"
#include "ship.h"
#include "../../Node.h"

using std::string;
using std::vector;

using namespace Helper;

class SpriteCharacterGame
{
public:
	SpriteCharacterGame();
	~SpriteCharacterGame();

	SpriteCharacterGame(bool a_bool);

	void GetKeys(ship a_ship);
	void SpawnNodes(int a_rows, int a_cols, int a_x, int a_y, int a_increment);
	void SpawnNodes(int a_rows, int a_cols, int a_x, int a_y, int a_increment, vector<Node*>* a_waypoint);
	void SpawnSystem(vector<Node*>* a_waypoint, int a_x, int a_y, int a_unit,int a_star, int a_planet, int a_moons, int a_satellite, int a_asteroid);
	void UpdateStarSystem(Matrix3 a_worldView,vector<Node*>* a_waypoint,Matrix3 CameraMat);



	vector<Node*> Waypoint;
	void ResetData();
	bool init();

protected:
	int m_stars;
	int m_planets;
	int m_moons;
	int m_satellites;
	int m_asteroids;
	bool m_initialized;

};