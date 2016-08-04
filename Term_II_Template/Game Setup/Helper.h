#pragma once
#include <iostream>
#include <time.h>
#include "string.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "sdlWindow.h"
#include "sdlResources.h"

namespace Helper
{
	void HELP_DrawSprite(SDL_Texture* Texture, float Frame, float DownFrame, float FrameSizeX, float FrameSizeY,
		float X, float Y, float SizeX, float SizeY, int Delay, float ANGLE);

	bool HELP_Keypresses(SDL_Scancode a_scancode);

	int HELP_Mousepresses(SDL_Event * e);

	int HELP_Random(int pmin, int pmax);

//	float HELP_Randomf(float pmin, float pmax);
};