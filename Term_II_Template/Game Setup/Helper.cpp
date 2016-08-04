#pragma once
#include "string.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "sdlWindow.h"
#include "sdlResources.h"
#include "Helper.h"
#include <time.h>
#include "main.h"


namespace Helper
{
	void HELP_DrawSprite(SDL_Texture* Texture, float Frame, float DownFrame, float FrameSizeX, float FrameSizeY, 
						 float X, float Y, float SizeX, float SizeY, int Delay, float ANGLE)
	{
		static int tmr = Delay;
		const int tmrReset = tmr;
		tmr--;
		static int i;
		static int j;
		if (Delay != 0) //which part to draw.
		{
			if (tmr == 0)
			{
				i++;
				j++;
				if (i >= Frame) i = 0;
				if (j >= DownFrame) j = 0;
				tmr = tmrReset;
			}
		}

		X -= SizeX / 2;
		Y -= SizeY / 2;

		SDL_Rect DimensionOne = { i * FrameSizeX, j * FrameSizeY,FrameSizeX,FrameSizeY };
		SDL_Rect DimensionTwo = { X,Y,SizeX,SizeY };
		Window::Draw(Texture, DimensionTwo, &DimensionOne, ANGLE);
	}

	bool HELP_Keypresses(SDL_Scancode a_scancode)
	{
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[a_scancode]) return true;
		return false;
	}

	int HELP_Mousepresses(SDL_Event * e)
	{		
		if (e->type == SDL_MOUSEBUTTONDOWN) 
		{ 
			switch (e->button.button)
			{
			case SDL_BUTTON_LEFT:
				return 1;
			case SDL_BUTTON_RIGHT:
				return 2;
			default:
				cout << "Dank Memes dot dat" << endl;
				return 0;
			}
		}
		return 0;
	}

	int HELP_Random(int pMin, int pMax)
	{
		return (rand() % (pMax + 1 - pMin)) + pMin;
	}

	//float HELP_Randomf(float pMin, float pMax)
	//{
	//	return (rand() % (pMax + 1 - pMin)) + pMin;
	//}

};
