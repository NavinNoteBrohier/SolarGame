#include "SpriteGameRoutines.h"
SpriteCharacterGame::SpriteCharacterGame()
{

}

SpriteCharacterGame::~SpriteCharacterGame()
{

}

SpriteCharacterGame::SpriteCharacterGame(bool a_bool)
{
	a_bool = false;
}

void SpriteCharacterGame::GetKeys(ship a_ship)
{
	Vector3 A(1,	 0,		 0);
	Vector3 B(0,	-1,		 0);
	Vector3 C(0,	 0,		 1);
	Vector3 D(0,	 0,		-1);
	
	if(HELP_Keypresses(SDL_SCANCODE_W)) a_ship.Moveship(A);
	if(HELP_Keypresses(SDL_SCANCODE_S)) a_ship.Moveship(B);
	if(HELP_Keypresses(SDL_SCANCODE_A))	a_ship.Moveship(C);
	if(HELP_Keypresses(SDL_SCANCODE_D))	a_ship.Moveship(D);
	
	if(HELP_Keypresses(SDL_SCANCODE_SPACE));
	
	if(HELP_Keypresses(SDL_SCANCODE_ESCAPE));
}

void SpriteCharacterGame::SpawnNodes(int a_rows, int a_cols, int a_x, int a_y, int a_increment)
{
	enum NodeType
	{
		STARTROOM = 1,ENDROOM = 1,EMPTYROOM,ASTEROIDROOM,ENEMYROOM
	};
	int k = 0;
	int layout[255][255];

	for (int i = 0; i < 255; i++)
	{
		for (int j = 0; j < 255; j++)
		{
			layout[i][j] = 0;
		}
	}

	for (int i = 0; i < a_rows; i++)
	{
		for (int j = 0; j < a_cols; j++)
		{
			layout[i][j] = HELP_Random(EMPTYROOM, ENEMYROOM);
			Waypoint.push_back(new Node(a_x + a_increment*j, a_y + a_increment * i, layout[i][j]));
			if		(layout[i][j] == EMPTYROOM)		{ Waypoint[k]->ChangeSprite(TM_NODEEMPTY); }
			else if (layout[i][j] == ASTEROIDROOM)	{ Waypoint[k]->ChangeSprite(TM_NODEASTEROID); }
			else if (layout[i][j] == ENEMYROOM)		{ Waypoint[k]->ChangeSprite(TM_NODEENEMY); };
			Waypoint[k]->SetSprite(50, 50, 0, 0, 50, 50, 0, 0.05f);
			k++;
		}
	}

	layout[0][0] = STARTROOM;
	Waypoint[0]->ChangeSprite(TM_NODE);
	layout[a_rows][a_cols] = ENDROOM;
	Waypoint[k-1]->ChangeSprite(TM_NODEEND);
}

void SpriteCharacterGame::SpawnNodes(int a_rows, int a_cols, int a_x, int a_y, int a_increment, vector<Node*>* a_waypoint)
{
	enum NodeType
	{
		STARTROOM = 1, ENDROOM = 1, EMPTYROOM, ASTEROIDROOM, ENEMYROOM
	};
	int k = 0;
	int layout[255][255];

	for (int i = 0; i < 255; i++)
	{
		for (int j = 0; j < 255; j++)
		{
			layout[i][j] = 0;
		}
	}

	for (int i = 0; i < a_rows; i++)
	{
		for (int j = 0; j < a_cols; j++)
		{
			layout[i][j] = HELP_Random(EMPTYROOM, ENEMYROOM);
			(*a_waypoint).push_back(new Node(a_x + a_increment*j, a_y + a_increment * i, 1 ));
			if (layout[i][j] == EMPTYROOM) { (*a_waypoint)[k]->ChangeSprite(TM_NODEEMPTY); }
			else if (layout[i][j] == ASTEROIDROOM) { (*a_waypoint)[k]->ChangeSprite(TM_NODEASTEROID); }
			else if (layout[i][j] == ENEMYROOM) { (*a_waypoint)[k]->ChangeSprite(TM_NODEENEMY); };
			(*a_waypoint)[k]->SetSprite(50, 50, 0, 0, 50, 50, 0, 0);
			(*a_waypoint)[k]->Index = k;
			k++;
		}
	}

	k = 0;

	for (int i = 0; i < a_rows; i++)
	{
		for (int j = 0; j < a_cols; j++)
		{
			if (j > 0)
			{
				(*a_waypoint)[k]->Connections[0] = (*a_waypoint)[k - 1];
			}

			if (j < a_cols - 1)
			{
				(*a_waypoint)[k]->Connections[1] = (*a_waypoint)[k + 1];
			}

			if (i > 0)
			{
				(*a_waypoint)[k]->Connections[2] = (*a_waypoint)[k - a_cols];
			}

			if (i < a_rows - 1)
			{
				(*a_waypoint)[k]->Connections[3] = (*a_waypoint)[k + a_cols];
			}
			(*a_waypoint)[k]->pathing = true;
			k++;
		}
	}

	layout[0][0] = STARTROOM;
	(*a_waypoint)[0]->ChangeSprite(TM_NODE);
	layout[a_rows][a_cols] = ENDROOM;
	(*a_waypoint)[k - 1]->ChangeSprite(TM_NODEEND);
}

int KL()
{
int i = 0;
while (i == 0) { i = HELP_Random(-1, 1); } 
return i;
};

void SpriteCharacterGame::SpawnSystem(vector<Node*>* a_waypoint, int a_x, int a_y, int a_unit, int a_star, int a_planet,int a_moons, 
	int a_satellite, int a_asteroid)
{
	float Rot = 0.00025;
	int ReqA = 100;
	//float Rot() { return HELP_Random(); };
	int k = 0;
	enum NodeType
	{
		EMPTY, STAR, PLANET, MOON, SATELLITE, ASTEROID
	};
	for (int i = 0; i <  a_star; i++)
	{
		(*a_waypoint).push_back(new Node(a_x , a_y,STAR));
		(*a_waypoint)[k]->ChangeSprite(TM_SUN);
		(*a_waypoint)[k]->SetNodeReq(0,Fuel);
		(*a_waypoint)[k]->Immortal = true;
		(*a_waypoint)[k]->m_ResourceAmount = ReqA;
		(*a_waypoint)[k]->SetSprite(110, 110, 0, 0, 110, 110, 0, Rot);
		m_stars++;
		k++;
		cout << "Star " << k << endl;
	}
	for (int i = 0; i < a_planet; i++)
	{
		int j = 0;
		while(j == 0){j = HELP_Random(-1, 1);}
		(*a_waypoint).push_back(new Node(KL() * i *(110) + (a_unit), KL() * i * (110) + (a_unit), PLANET));
		(*a_waypoint)[k]->ChangeSprite(TM_PLANETONE);
		for (int i = 0; i < a_waypoint->size(); i++) { if ((*a_waypoint)[k]->m_x == (*a_waypoint)[i]->m_x && (*a_waypoint)[k]->m_y == (*a_waypoint)[i]->m_y) { (*a_waypoint)[k]->m_x += HELP_Random(100,200); (*a_waypoint)[k]->m_y += HELP_Random(100, 200);} }
		(*a_waypoint)[k]->SetSprite(110, 110, 0, 0, 110/2, 110/2, 0, Rot + 0.00025 * KL());
		(*a_waypoint)[k]->SetParent(0);
		(*a_waypoint)[k]->SetNodeReq(0, Metal);
		(*a_waypoint)[k]->m_ResourceAmount = ReqA;
		if ((*a_waypoint)[k - 1]->m_x == (*a_waypoint)[k]->m_x) { (*a_waypoint)[k]->m_x *= -1; }
		if ((*a_waypoint)[k - 1]->m_y == (*a_waypoint)[k]->m_y) { (*a_waypoint)[k]->m_y *= -1; }
		m_planets++;
		k++;
		cout << "Planet " << k << endl;
	}
	for (int i = 0; i < a_moons; i++)
	{
		int j = 0;
		while (j == 0) { j = HELP_Random(-1, 1); }
		(*a_waypoint).push_back(new Node(KL() * i *(110) + (a_unit), KL() * i * (110) + (a_unit), MOON));
		(*a_waypoint)[k]->ChangeSprite(TM_MOONONE);
		(*a_waypoint)[k]->SetNodeReq(0, Fuel);
		for (int i = 0; i < a_waypoint->size(); i++) { if ((*a_waypoint)[k]->m_x == (*a_waypoint)[i]->m_x && (*a_waypoint)[k]->m_y == (*a_waypoint)[i]->m_y) { (*a_waypoint)[k]->m_x += HELP_Random(100, 200); (*a_waypoint)[k]->m_y += HELP_Random(100, 200); } }
		(*a_waypoint)[k]->SetSprite(110, 110, 0, 0, 110/3, 110/3, 0, Rot + 0.0025 * KL());
		(*a_waypoint)[k]->SetParent(HELP_Random(a_star + 1, a_planet));
		(*a_waypoint)[k]->m_ResourceAmount = ReqA;
		if ((*a_waypoint)[k - 1]->m_x == (*a_waypoint)[k]->m_x) { (*a_waypoint)[k]->m_x *= -1; }
		if ((*a_waypoint)[k - 1]->m_y == (*a_waypoint)[k]->m_y) { (*a_waypoint)[k]->m_y *= -1; }
		m_moons++;
		k++;
		cout << "Moon " << k << endl;
	}
	for (int i = 0; i < a_asteroid; i++)
	{
		int j = 0;
		while (j == 0) { j = HELP_Random(-1, 1); }
		(*a_waypoint).push_back(new Node(KL() * i *(110) + (a_unit), KL() * i * (110) + (a_unit), PLANET));
		(*a_waypoint)[k]->ChangeSprite(TM_PLANETONE);
		(*a_waypoint)[k]->SetNodeReq(0, Metal);
		for (int i = 0; i < a_waypoint->size(); i++) { if ((*a_waypoint)[k]->m_x == (*a_waypoint)[i]->m_x && (*a_waypoint)[k]->m_y == (*a_waypoint)[i]->m_y) { (*a_waypoint)[k]->m_x += HELP_Random(100, 200); (*a_waypoint)[k]->m_y += HELP_Random(100, 200); } }
		(*a_waypoint)[k]->SetSprite(110, 110, 0, 0, 110/4, 110/4, 0, Rot + 0.025 * KL());
		(*a_waypoint)[k]->SetParent(HELP_Random(a_star + 1, a_moons));
		(*a_waypoint)[k]->m_ResourceAmount = ReqA;
		if ((*a_waypoint)[k - 1]->m_x == (*a_waypoint)[k]->m_x) { (*a_waypoint)[k]->m_x *= -1; }
		if ((*a_waypoint)[k - 1]->m_y == (*a_waypoint)[k]->m_y) { (*a_waypoint)[k]->m_y *= -1; }
		m_asteroids++;
		k++;
		cout << "Asteroid " << k << endl;
	}

	(*a_waypoint).push_back(new Node(a_unit + 100, a_unit + 100, SATELLITE));
	(*a_waypoint)[k]->ChangeSprite(TM_BASESTATION);  
	(*a_waypoint)[k]->SetNodeReq(0, Home);
	(*a_waypoint)[k]->m_ResourceAmount = ReqA;
	(*a_waypoint)[k]->Immortal = true;
	for (int i = 0; i < a_waypoint->size(); i++) { if ((*a_waypoint)[k]->m_x == (*a_waypoint)[i]->m_x && (*a_waypoint)[k]->m_y == (*a_waypoint)[i]->m_y) { (*a_waypoint)[k]->m_x += HELP_Random(100, 200); (*a_waypoint)[k]->m_y += HELP_Random(100, 200); } }
	(*a_waypoint)[k]->SetSprite(150, 100, 0, 0, 150/2.5, 100/2.5, 0, Rot + 0.000050 * KL());
	(*a_waypoint)[k]->SetParent(a_star);
	if ((*a_waypoint)[k - 1]->m_x == (*a_waypoint)[k]->m_x) { (*a_waypoint)[k]->m_x *= -1; }
	if ((*a_waypoint)[k - 1]->m_y == (*a_waypoint)[k]->m_y) { (*a_waypoint)[k]->m_y *= -1; }
	m_satellites++;
	cout << "Satellite " << k << endl;

	bool DistanceCheck = false;
	while (DistanceCheck == false)
	{
		DistanceCheck = true;
		for (int i = 0; i < (*a_waypoint).size(); i++)
		{
			if ((*a_waypoint)[i]->m_x > 1250) { (*a_waypoint)[i]->m_x -= 500; DistanceCheck = false; }
			if ((*a_waypoint)[i]->m_y > 1250) { (*a_waypoint)[i]->m_y -= 500; DistanceCheck = false; }
			cout << "Distance Checking " << i <<  endl;
		};
	}
	cout << "Distance Checked " << endl;
	bool BB = false;
	bool AA = false;
	int j = 0;
	int i = 0;
	int L = 0;
	int M = 0;
	int H = a_waypoint->size();

	int Rotations = 0;

	while (BB == false)
	{
		BB = true;
		AA = false;
		while (AA == false)
		{
			AA = true;
			if (j == 0 && i == 0) { i++; AA == false;}
			else if (j == i) { j++; AA == false;}

			if (j >= a_waypoint->size()) { j = 0; }
			if (i >= a_waypoint->size()) { i = 0; }
			
			if ((*a_waypoint)[i]->m_x == (*a_waypoint)[j]->m_x && (*a_waypoint)[i]->m_y == (*a_waypoint)[j]->m_y)
			{
				(*a_waypoint)[i]->SetXY(HELP_Random(150,500),HELP_Random(150,500));
				AA == false;
				BB == false;
			}
			else { L++; BB = false; AA = false;}

			if (L >= a_waypoint->size()) {AA = true; }

			j++;
			Rotations++;
			if (Rotations > 5000)
			{ return; }
			cout << "Iteration "<< Rotations << endl;
		}
		M++;
		j = 0;
		L = 0;
		if (M == a_waypoint->size()) { BB = true; }
		i++;
		if (i > a_waypoint->size()) { i = 0; }
		Rotations++;
		if (Rotations > 5000)
		{ return; }
		cout << "Iteration " << Rotations << endl;
	}
	cout << "Iterated " << Rotations << " times" << endl;
	i = 0;
	j = 0;
	cout << "System created" << endl;
}

void SpriteCharacterGame::UpdateStarSystem(Matrix3 a_worldView,vector<Node*>* a_waypoint, Matrix3 CameraMat)
{
	enum NodeType
	{
		EMPTY, STAR, PLANET, MOON, SATELLITE, ASTEROID
	};
	int k = 0;
	for (int i = 0; i < (*a_waypoint).size(); i++)
	{
		if ((*a_waypoint)[k]->m_cost == STAR) { (*a_waypoint)[k]->UpdateNode(a_worldView, CameraMat); }
		else
		{
			(*a_waypoint)[k]->UpdateNode(a_worldView,	(*a_waypoint)[(*a_waypoint)[k]->m_parent]->NodeMat);
		}
		k++;
	}
}



void SpriteCharacterGame::ResetData()
{

}

bool SpriteCharacterGame::init()
{
	return true;
}
