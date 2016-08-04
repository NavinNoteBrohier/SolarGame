////////////////////////////////////////////////////////////////////////////////////////////////////////////
////																									////
////		Name:		menuScreen.cpp : SDL2 Template										     		////
////																									////
////		Purpose:	Screen which drives the startup and the menu for the games                      ////
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

#include "..\..\Game Setup\main.h"
#include "..\..\Game Setup\sdlWindow.h"
#include "..\..\Game Setup\sdlResources.h"


#include "..\screenManager.h"
#include "menuScreen.h"
#include "..\SpriteGame\SpriteGameScreen.h"


static string button2a = "Standard Game";
static string button2b = "One exposed Letter";
static string button2c = "Two exposed Letters";
static string button3 = "SpriteGame";
static string button4 = "Quit";

// unique name of all buttons
enum BUTTON_ID {
	B_NULL,
	B_START, B_TWO_PLAYER, B_SOMETHING, B_QUIT,
};


static ButtonData l_menuData[] = 
{
	{ ENTER_TXT_BUTTON, B_SOMETHING,	WB_CENTER, -1, 200, 311, 44, 228, 1, &button3 },		// 
	{ ENTER_TXT_BUTTON, B_QUIT,			WB_CENTER, -1, 250, 311, 44, 228, 1, &button4 },		// 
	{ ENTER_END, },
};

static BUTTON_STATE checkButtons(BUTTON_ID p_buttonId);
static BUTTON_RETURN actOnButton(BUTTON_ID p_buttonId, short shiftKey);
static void drawMenuScreen(SDL_Event * e);

//! openMenuScreen 
//! 20150426
void openMenuScreen()
{
	gSM.mBData = l_menuData;
	gSM.p_buttonCheck = checkButtons;
	gSM.p_buttonHit = actOnButton;
	gSM.p_drawScreen = drawMenuScreen;
	gSM.p_checkKeyboard = nullptr;
}

//! checkButtons 
//! 20150426
static BUTTON_STATE checkButtons(BUTTON_ID p_buttonId)
{
	switch (p_buttonId) {
	case B_START:
		return B_NORMAL;
	case B_TWO_PLAYER:
		return B_NORMAL;
	case B_SOMETHING:
		return B_NORMAL;
	case B_QUIT:
		return B_NORMAL;
	}
	return B_IGNORE;
}

//! actOnButton 
//! 20150426
static BUTTON_RETURN actOnButton(BUTTON_ID p_buttonId, short shiftKey)
{
	switch (p_buttonId) {
	case B_SOMETHING:
		OpenSpriteGame();
		return BUTTON_IS_DONE;
	case B_QUIT:
		gML.quit_game = true;
		return BUTTON_IS_DONE;
	}
	return BUTTON_DID_NOTHING;
}

//! drawMenuScreen 
//! 20150426
static void drawMenuScreen(SDL_Event * e)
{
	gSM.centreButtons();

	Window::setDrawColor(0x00, 0x00, 0x00, 0xff);
	Window::Clear();

	SDL_Rect win = Window::Box();

	SDL_Rect s1 = { 0, 0, 2048, 2048 };
	SDL_Rect d1 = { 0, 0, win.w, win.h };
	Window::Draw(TM_SPRITEPAPER, d1, &s1);

	// and the button data
	gSM.drawCurrentButtonsDisplay();

	char tempStr[256];
	static unsigned int count = 0;
	sprintf_s(tempStr, "Frame rate = %d", count++);
	Window::printString(10 + 4, 20, tempStr, cBlack, 30);
	Window::printString(10, 20 + 4, tempStr, cBlack, 30);
	Window::printString(10 + 4, 20 + 4, tempStr, cBlack, 30);
	Window::printString(10, 20, tempStr, cYellow, 30);

	// and update the current window rectangle
	Window::Present();
}


// end of menuScreen.cpp
