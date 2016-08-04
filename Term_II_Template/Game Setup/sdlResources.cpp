////////////////////////////////////////////////////////////////////////////////////////////////////////////
////																									////
////		Name:		sdlResources.cpp : SDL2 Template									     		////
////																									////
////		Purpose:	loads and unloads the resources for the game				                    ////
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

#include "main.h"
#include "sdlWindow.h"
#include "sdlResources.h"

SDL_Surface *SM_TANKS, *SM_CURSOR;
SDL_Texture *TM_TANKS, *TM_HANGPAPER,*TM_WALLPAPER, *TM_GOTONEXT, *TM_HANGMAN, *TM_SMALLRECT, *TM_TEXT_BUTTON, *TM_HANG_BUTTONS, 
*TM_FIRE,*TM_FARM, *TM_TRACK, *TM_SPRITEPAPER, *TM_SPRITECHARACTER, *TM_SHIP, *TM_WAKE, *TM_NODE, *TM_NODEEND, *TM_NODEENEMY, 
*TM_NODEASTEROID,*TM_NODEEMPTY,*TM_PODSHIP,*TM_SUN, *TM_PLANETONE, *TM_BASESTATION, *TM_MOONONE, *TM_DEPLETED;


Mix_Chunk *WAVE_SCRATCH, *WAVE_HIGH, *WAVE_MEDIUM, *WAVE_LOW, *WAVE_CAMERA;
Mix_Music *WAVE_MUSIC;

// music from
//http://www.danosongs.com/package/

static loadMusic lmus[] = {
	{ LOAD_MUSIC, &WAVE_MUSIC, "/Resources/Sounds/sunspark.mp3", ERROR_MUSIC },

	{ LOAD_END },
};

static loadSounds lsnd[] = {
	{ LOAD_SOUND, &WAVE_SCRATCH, "/Resources/Sounds/scratch.wav", ERROR_SOUNDS },
	{ LOAD_SOUND, &WAVE_HIGH, "/Resources/Sounds/high.wav", ERROR_SOUNDS },
	{ LOAD_SOUND, &WAVE_MEDIUM, "/Resources/Sounds/medium.wav", ERROR_SOUNDS },
	{ LOAD_SOUND, &WAVE_LOW, "/Resources/Sounds/low.wav", ERROR_SOUNDS },
	{ LOAD_SOUND, &WAVE_CAMERA, "/Resources/Sounds/cameraShutter.wav", ERROR_SOUNDS },

	{ LOAD_END },
};

static loadSurface ls[] = {
	{ LOAD_SURFACE, &SM_TANKS, "/Resources/Bitmaps/Picts/Tanks.png", ERROR_SURFACE },
	{ LOAD_SURFACE, &SM_CURSOR, "/Resources/Bitmaps/Picts/Cursors.png", ERROR_SURFACE },

	{ LOAD_END },
};

static loadTexture lt[] = {
	{ LOAD_TEXTURE, &TM_TANKS, "/Resources/Bitmaps/Picts/Tanks.png", ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_WALLPAPER, "/Resources/Bitmaps/Picts/startwall.png", ERROR_TEXTURE },
	// hangman
	{ LOAD_TEXTURE, &TM_HANGPAPER, "/Resources/Bitmaps/Hangman/wallpaper.png", ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_GOTONEXT, "/Resources/Bitmaps/Hangman/gotoNext.png", ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_HANGMAN, "/Resources/Bitmaps/Hangman/Hang0.png",		ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_SMALLRECT, "/Resources/Bitmaps/Hangman/smallRect.png", ERROR_TEXTURE },
	// general
	{ LOAD_TEXTURE, &TM_TEXT_BUTTON, "/Resources/Bitmaps/Picts/textButtons.png", ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_HANG_BUTTONS, "/Resources/Bitmaps/Picts/buttonsUtil.png", ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_FIRE, "/Resources/Bitmaps/Picts/fire.png", ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_FARM, "/Resources/Bitmaps/New/farm_house_sprite.png", ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_TRACK, "/Resources/Bitmaps/New/track.png", ERROR_TEXTURE },
	//SpriteGame
	{ LOAD_TEXTURE, &TM_SPRITEPAPER, "/Resources/Bitmaps/SpriteGame/Starscape1.png", ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_SPRITECHARACTER,"/Resources/Bitmaps/SpriteGame/knight.png",ERROR_TEXTURE},
	{ LOAD_TEXTURE, &TM_SHIP,"/Resources/Bitmaps/SpriteGame/ship.png",ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_WAKE,"/Resources/Bitmaps/SpriteGame/wake.png",ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_NODE,"/Resources/Bitmaps/SpriteGame/node.png",ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_NODEEND,"/Resources/Bitmaps/SpriteGame/nodeEnd.png",ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_NODEENEMY,"/Resources/Bitmaps/SpriteGame/nodeEnemy.png",ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_NODEASTEROID,"/Resources/Bitmaps/SpriteGame/nodeAsteroid.png",ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_NODEEMPTY,"/Resources/Bitmaps/SpriteGame/nodeEmpty.png",ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_PODSHIP,"/Resources/Bitmaps/SpriteGame/Podship.png",ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_SUN,"/Resources/Bitmaps/SpriteGame/sun.png",ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_PLANETONE,"/Resources/Bitmaps/SpriteGame/rockyone.png",ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_BASESTATION,"/Resources/Bitmaps/SpriteGame/BaseStation.png",ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_MOONONE,"/Resources/Bitmaps/SpriteGame/MoonOne.png",ERROR_TEXTURE },
	{ LOAD_TEXTURE, &TM_DEPLETED,"/Resources/Bitmaps/SpriteGame/Destroyed.png",ERROR_TEXTURE },
	{ LOAD_END },
};

static loadCursor lc[] = {
	{ CURS_ARROW,	0, 1, 32, 32, 00, 00, 367, 1, 100 },

	{ CURS_POINTER, 0, 1, 13, 21, 00, 00, 1, 1, 100 },

	{ CURS_SELECT,	0, 6, 32, 32, 16, 16, 17, 1, 100 },
	{ CURS_NULL,	0, 6, 32, 32, 16, 16, 50, 1, 100 },
	{ CURS_NULL,	0, 6, 32, 32, 16, 16, 83, 1, 100 },
	{ CURS_NULL,	0, 6, 32, 32, 16, 16, 116, 1, 100 },
	{ CURS_NULL,	0, 6, 32, 32, 16, 16, 83, 1, 100 },
	{ CURS_NULL,	0, 6, 32, 32, 16, 16, 50, 1, 100 },

	{ CURS_MAX },
};

Resources gRes;

//! Resources : constructor
//! 20150426
Resources::Resources()
{
	// initialize the surfaces textures sounds and cursors
	int count = 0;
	while (ls[count].type == LOAD_SURFACE)
	{
		*ls[count].sur = nullptr;
		count++;
	}
	// and the textures
	count = 0;
	while (lt[count].type == LOAD_TEXTURE)
	{
		*lt[count].tex = nullptr;
		count++;
	}
	// music
	count = 0;
	while (lmus[count].type == LOAD_MUSIC)
	{
		*lmus[count].mus = nullptr;
		count++;
	}
	// sounds
	count = 0;
	while (lsnd[count].type == LOAD_SOUND)
	{
		*lsnd[count].snd = nullptr;
		count++;
	}

	// cursors
	this->soundOn = true;
	this->mCursor_time = 0;
	this->mCursorNumber = 0;
	this->mCurrentCursorType = CURS_NULL;
}

//! Resources : deconstructor
//! 20150426
Resources::~Resources()
{
}

//! Resources : loadResources - load all resources returning error number if bad load
//! 20150426
int Resources::loadResources()
{
	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return ERROR_MUSIC_SETUP;
	}

	// load surfaces
	int count = 0;
	while (ls[count].type == LOAD_SURFACE)
	{
		if (ls[count].name != "") {
			string loadName = *gML.getStartDirectory() + ls[count].name;

			*ls[count].sur = IMG_Load(loadName.c_str());

			//If there was a problem in loading the background
			if (*ls[count].sur == nullptr)
				return ls[count].errorCode + count;
		}
		count++;
	}

	// load textures
	count = 0;
	while (lt[count].type == LOAD_TEXTURE)
	{
		if (lt[count].name != "") {
			string loadName = *gML.getStartDirectory() + lt[count].name;

			*lt[count].tex = Window::LoadThisImage(loadName.c_str());
			if (*lt[count].tex == nullptr) return lt[count].errorCode + count;
		}
		count++;
	}
	// load music
	count = 0;
	while (lmus[count].type == LOAD_MUSIC)
	{
		if (lmus[count].name != "") {
			string loadName = *gML.getStartDirectory() + lmus[count].name;

			*lmus[count].mus = Mix_LoadMUS(loadName.c_str());
			//char tempStr[256];
			//sprintf_s(tempStr, "Mix_LoadMUS(\"music.mp3\"): %s\n", Mix_GetError());
			if (*lmus[count].mus == nullptr) return lmus[count].errorCode + count;
		}
		count++;
	}

	// load sounds
	count = 0;
	while (lsnd[count].type == LOAD_SOUND)
	{
		if (lsnd[count].name != "") {
			string loadName = *gML.getStartDirectory() + lsnd[count].name;

			//music = Mix_LoadMUS("beat.wav");

			*lsnd[count].snd = Mix_LoadWAV(loadName.c_str());
			if (*lsnd[count].snd == nullptr) return lsnd[count].errorCode + count;
		}
		count++;
	}

	if (SM_CURSOR == nullptr) return ERROR_CURSORS;

	// load cursors
	count = 0;
	while (lc[count].name != CURS_MAX)
	{
		lc[count].cursorNumber = count;
		lc[count].loaded = false;
		// create a blank surface for the curspr
		Uint32 rmask, gmask, bmask, amask;
		/* SDL interprets each pixel as a 32-bit number, so our masks must depend
		on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#endif
		// ETBD change cursors to vector array
		SDL_Surface *TempSur = SDL_CreateRGBSurface(SDL_SWSURFACE, lc[count].cWidth, lc[count].cHeight, 32, rmask, gmask, bmask, amask);
		// this creates and empty army sheet if that is what we want to do
		if (TempSur == nullptr) return EXIT_FAILURE;
		// copy the cursor from the bitmap to this surface
		SDL_Rect to, from;
		from.x = lc[count].sourceX;
		from.y = lc[count].sourceY;
		from.w = lc[count].cWidth;
		from.h = lc[count].cHeight;
		to.x = 0;
		to.y = 0;
		to.w = from.w;
		to.h = from.h;

		SDL_Surface *fromSur;
		fromSur = SM_CURSOR;

		SDL_BlitSurface(fromSur, &from, TempSur, &to);
		lc[count].cursor = SDL_CreateColorCursor(TempSur, lc[count].hotX, lc[count].hotY);

		SDL_FreeSurface(TempSur);

		lc[count].loaded = true;
		count++;
	}
	return EXIT_SUCCESS;
}

//! Resources : unLoadResources - unload all resources that have been loaded
//! 20150426
void Resources::unLoadResources()
{
	// unload surfaces
	int count = 0;
	while (ls[count].type == LOAD_SURFACE)
	{
		if (*ls[count].sur != nullptr)
			SDL_FreeSurface(*ls[count].sur);
		*ls[count].sur = nullptr;
		count++;
	}
	// unload textures
	count = 0;
	while (lt[count].type == LOAD_TEXTURE)
	{
		if (*lt[count].tex != nullptr)
			SDL_DestroyTexture(*lt[count].tex);
		*lt[count].tex = nullptr;
		count++;
	}
	// unload music
	count = 0;
	while (lmus[count].type == LOAD_MUSIC)
	{
		if (lmus[count].mus != nullptr)
			Mix_FreeMusic(*lmus[count].mus);
		lmus[count].mus = nullptr;
		count++;
	}

	// unload sounds
	count = 0;
	while (lsnd[count].type == LOAD_SOUND)
	{
		if (lsnd[count].snd != nullptr)
			Mix_FreeChunk(*lsnd[count].snd);
		lsnd[count].snd = nullptr;
		count++;
	}

	// unload cursors
	count = 0;
	while (lc[count].name != CURS_MAX)
	{
		if (lc[count].loaded) {
			SDL_FreeCursor(lc[count].cursor);
			lc[count].loaded = false;
		}
		count++;
	}
}

//! Resources : setCursor - set the current cursor
//! 20150426
void Resources::setCursor(CursorType p_type)
{
	int count = 0;
	for (;;) {
		if (lc[count].name == CURS_MAX) return;
		if (lc[count].name == p_type) {
			// got it
			if (!lc[count].loaded) return;

			SDL_SetCursor(lc[count].cursor);

			mCursor_time = SDL_GetTicks();
			mCursorNumber = count;
			mCurrentCursorType = p_type;
			return;
		}
		count++;
	}
}

//! Resources : setCursor - set the current cursor
//! 20150426
void Resources::setCursorNumber(int  p_number)
{
	if (!lc[p_number].loaded) return;

	SDL_SetCursor(lc[p_number].cursor);
	mCursor_time = SDL_GetTicks();
	mCursorNumber = p_number;
}

//! Resources : UpdateCursor - called from main loop to update animated cursors
//! 20150426
void Resources::UpdateCursor()
{
	if (mCurrentCursorType == CURS_NULL) return;
	if (lc[mCursorNumber].maxFrames <= 1) return;


	if (SDL_GetTicks() - mCursor_time < lc[mCursorNumber].speed*1.5) return;

	// check the next one is ok
	if (lc[mCursorNumber + 1].name == CURS_NULL) {
		// set to next animation
		setCursorNumber(mCursorNumber + 1); 
	}
	else {
		// start again
		setCursorNumber(mCurrentCursorType);
	}
}

//! Resources : playSound - play this sound
//! 20150426
void Resources::playSound(Mix_Chunk *p_sound)
{
	if (!soundOn) return;

	Mix_PlayChannel(-1, p_sound, 0);
}

//! Resources : playMusic - play this music
//! 20150426
void Resources::playMusic(Mix_Music *p_music)
{
	if (!soundOn) return;

	Mix_PlayMusic(p_music, -1);
}

SDL_Color GetPixelOne(SDL_Surface* pSurface, int x, int y)
{
	SDL_Color color;
	Uint32 col = 0;

	//determine position
	char* pPosition = (char*)pSurface->pixels;

	//offset by y
	pPosition += (pSurface->pitch * y);

	//offset by x
	pPosition += (pSurface->format->BytesPerPixel * x);

	//copy pixel data
	memcpy(&col, pPosition, pSurface->format->BytesPerPixel);

	//convert color
	SDL_GetRGB(col, pSurface->format, &color.r, &color.g, &color.b);
	return (color);
}

// end of loadResources.cpp