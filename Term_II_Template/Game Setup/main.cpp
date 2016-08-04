////////////////////////////////////////////////////////////////////////////////////////////////////////////
////																									////
////		Name:		main.cpp : SDL2 Template											     		////
////																									////
////		Purpose:	Provides the means to quickly get into the coming assessments                   ////
////																									////
////		Author:		Roger Keating	: 22/04/2016													////
////																									////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <ctime>
#include <windows.h>
#include <math.h>
#include <string>
#include <vector>

#include "Helper.h"
#include "main.h"
#include "sdlWindow.h"
#include "sdlResources.h"
#include "../Screens/MainMenu/menuScreen.h"
#include "../Screens/screenManager.h"
#include <direct.h>
#include <time.h>

using namespace std;

MainLoop gML;

// a useful tool for debugging - see SDLK_1
FILE *fDebug;
#define DEBUGMSG(str) {char charStr[256]; sprintf_s(charStr, "%s\\DEBUG.txt", gML.getStartDirectory()->c_str());fopen_s(&fDebug, charStr,"at");fprintf(fDebug,str);fclose(fDebug);} 

//! MainLoop main - you have to start somewhere
//! 20150426
int main(int argc, char* args[])
{

	int errorNum = EXIT_SUCCESS;

	// initialize randomizer
	srand((unsigned)time(NULL));


	// get the current working directory
	char cCurrentPath[FILENAME_MAX];
	if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
	{
		return errno;
	}
	gML.setStartDirectory(cCurrentPath);

	SDL_Rect toThis{ 0, 0, 1680, 1050};
	Window::setWindowSize(toThis, SCREEN_WINDOW);

	// initialize the system and bring up a window
	Window::Init("Project");

	// load all the resources
	if ((errorNum = gRes.loadResources()) != EXIT_SUCCESS) 
	{
		gML.quit_game = true;
	}
	else
	{
		gRes.setCursor(CURS_SELECT);
		SDL_ShowCursor(SDL_ENABLE);
		if (!gML.quit_game)
		{
			// start game music
			Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
#if 0
			if (Mix_FadeInMusic(WAVE_MUSIC, -1, 5000) == -1)
			{
				//printf("Mix_FadeInMusic: %s\n", Mix_GetError());
				// well, there's no music, but most games don't break without music...
			}
#endif
			//gRes.playSound(WAVE_SCRATCH);
		}
	}
	// open this screen
	openMenuScreen();

	// initialise the cursor
	Window::initCursor(true);

	// the main game loop
	while (!gML.quit_game)
	{
		SDL_Event e;

		gRes.UpdateCursor();

		static bool leftB = false;
		static bool rightB = false;
		static bool leftPressed = false;
		static int smx = 0, smy = 0, mx = 0, my = 0;
		bool skip = false;
		bool buttonUp = false;
		bool buttonAction = false;
		bool shiftKey = false;

		if (gSM.p_checkKeyboard != nullptr) 
		{
			(*gSM.p_checkKeyboard)(&e);
		}

		while (SDL_PollEvent(&e))
		{

			if (e.type == SDL_WINDOWEVENT)
			{
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_MINIMIZED:
					break;
				case SDL_WINDOWEVENT_RESTORED:
					break;
				}
			}

			if (e.type == SDL_WINDOWEVENT_FOCUS_LOST)
			{
				skip = true;
			}

			//If user closes the window
			if (e.type == SDL_QUIT) 
			{
				gML.quit_game = true;
				break;
			}

			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym) 
				{
				case SDLK_PRINTSCREEN:
					Window::screenShot();
					break;
				case SDLK_1:
					break;
				case SDLK_2:
					break;
				case SDLK_3:
					break;
				case SDLK_4:
					break;
				case SDLK_5:
					break;

				case SDLK_d:
					break;
				case SDLK_a:
					break;
				case SDLK_f:
					Window::toggle_fullscreen();
					break;
				case SDLK_s:
					break;
				case SDLK_p:
					break;
					//For quitting, escape key
				case SDLK_ESCAPE:
#ifdef DEBUG_MODE
					gML.quit_game = true;
#endif
					break;
				case SDLK_F4:
				{
					// ALT_F4 will automatically quit the game
					const Uint8 *state = SDL_GetKeyboardState(NULL);
					if (!state[SDL_SCANCODE_LALT]) break;
				}
				gML.quit_game = true;
				break;
				case SDLK_LCTRL:
					break;
				case SDLK_RSHIFT:
				case SDLK_LSHIFT:
					break;
#if 0
				case SDLK_RIGHT:
					scrollWithKeys(MAP_SCROLL_LEFT);
					break;
				case SDLK_LEFT:
					scrollWithKeys(MAP_SCROLL_RIGHT);
					break;
				case SDLK_UP:
					scrollWithKeys(MAP_SCROLL_DOWN);
					break;
				case SDLK_DOWN:
					scrollWithKeys(MAP_SCROLL_UP);
					break;
#endif
				default:
					break;
				}
			}

			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				leftPressed = true;
				leftB = (e.button.button == SDL_BUTTON_LEFT);
				rightB = (e.button.button == SDL_BUTTON_RIGHT);
				buttonUp = false;

				smx = e.button.x;
				smy = e.button.y;
				mx = e.button.x;
				my = e.button.y;
				if (!buttonAction) 	gSM.depressCurrentButtons(shiftKey, e);

				buttonAction = true;
				break;
			}

			if (e.type == SDL_MOUSEBUTTONUP)
			{
				leftB = false;
				rightB = false;
				leftPressed = false;
				buttonUp = true;

				smx = 0;
				smy = 0;
				mx = 0;
				my = 0;
				if (!buttonAction) skip = gSM.releaseCurrentButtons(shiftKey, e);
				buttonAction = true;
				break;
			}

			//The mouse offsets
			if (e.type == SDL_MOUSEMOTION)
			{
				if (!buttonAction) gSM.moveCurrentButtons(shiftKey, e);
				buttonAction = true;
				break;
			}
		}

		(*gSM.p_drawScreen)(&e);
	}

	//Free resources and close SDL
	gRes.unLoadResources();

	// release SDL 
	Window::Quit();
	return errorNum;
}

//! MainLoop : MainLoop - constructor
//! 20150426
MainLoop::MainLoop()
{
	this->start_time		= SDL_GetTicks();
	this->quit_game			= false;
	this->mStartDirectory	= "";
}

//! MainLoop : MainLoop - deconstructor
//! 20150426
MainLoop::~MainLoop()
{
}

//! MainLoop : setStartDirectory - record the start directory
//! 20150426
void MainLoop::setStartDirectory(char *p_str)
{
	mStartDirectory = p_str;
}

//! MainLoop : getStartDirectory - get the start directory
//! 20150426
string * MainLoop::getStartDirectory()
{
	return &mStartDirectory;
}

// end of main.cpp