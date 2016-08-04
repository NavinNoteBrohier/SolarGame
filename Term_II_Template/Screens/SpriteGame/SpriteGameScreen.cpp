#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <iostream>
#include <fstream>
#include "SpriteGameScreen.h"
#include "SpriteGameRoutines.h"
#include "ship.h"
#include "../../Entity.h"
#include "..\screenManager.h"
#include "..\MainMenu\menuscreen.h"
#include "..\..\Game Setup\main.h"
#include "..\..\Game Setup\helper.h"
#include "..\..\Game Setup\sdlWindow.h"
#include "..\..\Game Setup\sdlResources.h"
#include "ship.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////////
//																		///
//	Navin Brohier														////
//	navin.brohier@gmail.com || navinnotebrohier.github.io				////
//																		///
//////////////////////////////////////////////////////////////////////////



SpriteCharacterGame Game;
// this is the name at the top of the screen
static string SpriteName = "Sprite Game";
SpriteCharacterGame game(true);
using namespace Helper;

Matrix3 WorldView(1, 0, 0, 0, 1, 0,0,0, 1);

Entity PlayerOne;

vector<Node*> SystemOne;

vector <int*> IdleShips;

vector<ship*> NavShip;

vector<Node*> PathfindingNodes;

ship PathfindingShip(TM_SHIP,1,1,50,50,88,77,0,0,false);
 
ship CameraShip(TM_SHIP, 1680 / 2, 1050 / 2, 0, 0, 0, 0, 0, 0, true);

bool MousePressed = false;

char tempStr[256];

enum BUTTON_ID {
	B_NULL,
	B_ONE, B_BACK, B_QUIT
};

static ButtonData l_gameData[] = {
	{ ENTER_BUTTON, B_QUIT, WB_MAIN_TOP_INFO, 969, 20, 42, 42, 143, 1 },
	{ ENTER_BUTTON, B_BACK, WB_MAIN_TOP_INFO, 20, 60, 36, 36, 57, 1 },
	{ ENTER_END, },
};

static BUTTON_STATE checkButtons(BUTTON_ID p_buttonId)
{
	switch (p_buttonId) {
	case B_QUIT:
		return B_NORMAL;
	}
	return B_IGNORE;
}

//! drawScreen - standard draw screen routine
static void drawScreen(SDL_Event* e)
{
	SDL_Rect winSize = Window::Box();



#pragma region //Scene
	SDL_Rect s1 = { 0, 0,1680, 1050 };
	SDL_Rect d1 = { 0, 0, 1680, 1050 };
	Window::Draw(TM_SPRITEPAPER, d1, &s1);
#pragma endregion

	Game.UpdateStarSystem(WorldView, &SystemOne , CameraShip.ShipMat);
	CameraShip.UpdateShip(WorldView);

#pragma region // Solar System

	for (int i = 0; i < (int)NavShip.size(); i++)
	{
		NavShip[i]->UpdateShip(WorldView);
	}

	if (PathfindingNodes.size() == 0)
	{
		cout << "zero Pathing" << endl;
	}
	else if (SystemOne.size() == 0)
	{
		cout << "zero System" << endl;
	}

	PlayerOne.updateEntity(&IdleShips);
#pragma endregion

 #pragma region //Debug 
	static bool Debugging = false;
	if (HELP_Keypresses(SDL_SCANCODE_HOME)) { Debugging = true; };
	if(Debugging == true)
	{

		if (HELP_Keypresses(SDL_SCANCODE_INSERT)) { Debugging = false; }
		sprintf_s(tempStr, "Ship Angle = %d", CameraShip.ReturnAngle());
		Window::printString(10 + 4, 20, tempStr, cBlue, 15); 

		sprintf_s(tempStr, "Q = %d", HELP_Keypresses(SDL_SCANCODE_Q));
		Window::printString(10 + 4, 80, tempStr, cBlue, 15);

		sprintf_s(tempStr, "E = %d", HELP_Keypresses(SDL_SCANCODE_E));
		Window::printString(10 + 4, 100, tempStr, cBlue, 15);

		sprintf_s(tempStr, "W = %d", HELP_Keypresses(SDL_SCANCODE_W));
		Window::printString(10 + 4, 120, tempStr, cBlue, 15);

		sprintf_s(tempStr, "S = %d", HELP_Keypresses(SDL_SCANCODE_S));
		Window::printString(10 + 4, 140, tempStr, cBlue, 15);

		sprintf_s(tempStr, "A = %d", HELP_Keypresses(SDL_SCANCODE_A));
		Window::printString(10 + 4, 160, tempStr, cBlue, 15);

		sprintf_s(tempStr, "D = %d", HELP_Keypresses(SDL_SCANCODE_D));
		Window::printString(10 + 4, 180, tempStr, cBlue, 15);

		sprintf_s(tempStr, "Z = %d", HELP_Keypresses(SDL_SCANCODE_Z));
		Window::printString(10 + 4, 200, tempStr, cBlue, 15);

		sprintf_s(tempStr, "X = %d", HELP_Keypresses(SDL_SCANCODE_X));
		Window::printString(10 + 4, 220, tempStr, cBlue, 15);

		sprintf_s(tempStr, "C = %d", HELP_Keypresses(SDL_SCANCODE_C));
		Window::printString(10 + 4, 240, tempStr, cBlue, 15);

		sprintf_s(tempStr, "V = %d", HELP_Keypresses(SDL_SCANCODE_V));
		Window::printString(10 + 4, 260, tempStr, cBlue, 15);

		sprintf_s(tempStr, "1 = %d", HELP_Keypresses(SDL_SCANCODE_1));
		Window::printString(10 + 4, 280, tempStr, cBlue, 15);

		sprintf_s(tempStr, "0 = %d", HELP_Keypresses(SDL_SCANCODE_0));
		Window::printString(10 + 4, 300, tempStr, cBlue, 15);
	}
#pragma endregion

#pragma region // Pathfinding
	static bool Pathfinding = false;
	if (HELP_Keypresses(SDL_SCANCODE_4)) { Pathfinding = true; };
	if (Pathfinding == true)
	{
		if (HELP_Keypresses(SDL_SCANCODE_5)) { Pathfinding = false; }

			for (int i = 0; i < (int)PathfindingNodes.size(); i++)
			{
				PathfindingNodes[i]->UpdateNode(WorldView);
			}
			
			
			int imX, imY;
			SDL_GetMouseState(&imX, &imY);

		

		if (HELP_Mousepresses(e) == 2 && MousePressed == false)
			{
				MousePressed = true;
				for (int i = 0; i < (int)PathfindingNodes.size(); i++)
				{
					if (PathfindingNodes[i]->m_x - 25 <= imX && PathfindingNodes[i]->m_x + 25 >= imX && 
						PathfindingNodes[i]->m_y - 25 <= imY && PathfindingNodes[i]->m_y + 25 >= imY)
					{
						PathfindingShip.StartDes = PathfindingNodes[PathfindingShip.Destination];
						PathfindingShip.EndDes = PathfindingNodes[i];
						PathfindingShip.Path = Pathing::FindPath(PathfindingShip.StartDes, PathfindingShip.EndDes,&PathfindingShip);
							if (PathfindingShip.Path.size() > 0)
							{
								PathfindingShip.Destination = PathfindingShip.Path[0]->Index;
							}
						
						}
				}
				cout << PathfindingShip.EndDes << endl;
				
			}
			
			PathfindingShip.UpdateShip(WorldView);
			if ((*e).button.type == SDL_MOUSEBUTTONUP)
			{
				MousePressed = false;
			}
	}

#pragma endregion
	// and update the current window rectangle

	Window::Present();
}

void OpenSpriteGame()
{
	if (!game.init())
	{
		cout << "Error: Game init() failed." << endl;
		return;
	}

	Game.SpawnSystem(&SystemOne, 0, 0, 100, 1, 10, 10, 1,2);
	for (int j = 0; j < 10; j++)
	{
		NavShip.push_back(new ship(TM_PODSHIP, 500, 500, 25, 25, 25, 25, 0, 0, false));
		NavShip[j]->SetTarget(&SystemOne);
		NavShip[j]->m_tex = TM_PODSHIP;
		NavShip[j]->SetTopSpeed(HELP_Random(1,8));
	}
	
	PathfindingShip.SetTopSpeed(4);
	PathfindingShip.m_tex = TM_SHIP;
	PathfindingShip.SetTarget(&PathfindingNodes);
	PathfindingShip.Pathing = true;
	PathfindingShip.Destination = 0;

	PlayerOne.Construct(&SystemOne,&NavShip,&IdleShips);



	Game.SpawnNodes(5, 5, 130, 130, 105, &PathfindingNodes);


	gSM.mBData = l_gameData;
	gSM.p_drawScreen = drawScreen;
	gSM.mTexture = TM_SPRITEPAPER;
	gSM.mTexture = TM_HANG_BUTTONS;
}  
