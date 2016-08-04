////////////////////////////////////////////////////////////////////////////////////////////////////////////
////																									////
////		Name:		screenManager.cpp : SDL2 Template									     		////
////																									////
////		Purpose:	Handles buttons for the various screens											////
////																									////
////		Author:		Roger Keating	: 22/04/2016													////
////																									////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

//#include "Timer.h"
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <ctime>
#include <windows.h>
#include <math.h>
#include <string>
#include <vector>

#include "..\Game Setup\main.h"
#include "..\Game Setup\sdlWindow.h"
#include "..\Game Setup\sdlResources.h"

#include "screenManager.h"
///#include "menuScreen.h"

Screen_Manager gSM;

SDL_Color	cWhite = { 255, 255, 255 }, cRed = { 255, 255, 0 }, cYellow = { 255, 255, 0 }, cLtGreen = { 0, 255, 0 };
SDL_Color	cGreen = { 0, 192, 0 }, cDkGreen = { 0, 128, 0 }, cCyan = { 0, 255, 255 }, cBlue = { 64, 64, 255 };
SDL_Color	cPurple = { 128, 0, 128 }, cText = { 192, 192, 0 }, cBlack = { 0, 0, 0 }, cDkGray = { 64, 64, 64 };
SDL_Color	cGray = { 128, 128, 128 }, cLtGray = { 192, 192, 192 }, cTransparent = { 255, 0, 255 }, cOrange = { 255, 64, 64 }, cBrown = { 140, 35, 15 };

//! Screen_Manager  constructor
//! 20150426
Screen_Manager::Screen_Manager()
{
	this->mDragButton = NON_SELECTED;
}

//! Screen_Manager  depressCurrentButtons the mouse has been depressed ... deal with it
//! 20150426
void Screen_Manager::depressCurrentButtons(bool shiftKey, SDL_Event e)
{
	if (p_drawScreen == NULL) return;

	// find out where the mouse is
	SDL_GetMouseState(&miX, &miY);

	checkCurrentButtons(shiftKey, e);
}

//! Screen_Manager  moveCurrentButtons the mouse has been moved ... deal with it
//! 20150426
void Screen_Manager::moveCurrentButtons(short shiftKey, SDL_Event e)
{
	if (p_drawScreen == NULL) return;

	checkCurrentButtons(shiftKey, e);
}

//! Screen_Manager  releaseCurrentButtons the mouse has been released ... deal with it
//! 20150426
bool Screen_Manager::releaseCurrentButtons(short shiftKey, SDL_Event e)
{
	if (p_drawScreen == NULL) return false;

	if (mDragButton != NON_SELECTED) {
		mBData[mDragButton].fDragged = false;
	}
	checkCurrentButtons(shiftKey, e);
	return false;
}

//! Screen_Manager  actOnActiveButton the button is active ... go do something
//! 20150426
void Screen_Manager::actOnActiveButton(ButtonData *p_ed, short p_shiftOn)
{
	short buttonType;
	BUTTON_ID buttonId = p_ed->name;

	if (buttonType = (*p_buttonHit)(buttonId, p_shiftOn)) {
	}
}

//! Screen_Manager  checkCurrentButtons see if anything needs to be done
//! 20150426
void Screen_Manager::checkCurrentButtons(short shiftKey, SDL_Event e)
{
	if (p_drawScreen == NULL) return;

	short depressed, selected;

	// find out where the mouse is
	SDL_GetMouseState(&miX, &miY);

	mLeftB = (e.button.button == SDL_BUTTON_LEFT);
	mRightB = (e.button.button == SDL_BUTTON_RIGHT);

	if (e.type == SDL_MOUSEBUTTONUP)
	{
		//mouseDownNow = false;
		short selected, depressed;
		if (mLeftB && (selected = returnSelectedButton(&depressed)) != NON_SELECTED)
		{
			if (depressed)
			{
				actOnActiveButton(&mBData[selected], shiftKey);
				//return true;
			}
		}
	}

	if ((e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) && mLeftB) 
	{
		short over = returnOverButton();
		if ((over = returnSelectedButton(&depressed)) != NON_SELECTED) 
		{
			switch (mBData[over].id) 
			{
			case ENTER_PAD:
				actOnActiveButton(&mBData[over], false);
				break;
			}
		}
	}

	switch (e.type) 
	{
	case SDL_MOUSEMOTION:
		break;
	case SDL_MOUSEBUTTONDOWN:
		//rightMouseDown = false;
		selected = returnSelectedButton(&depressed);

		if ((mDragButton = selected) == NON_SELECTED) break;
		if (depressed) break;
		setDepressedButton(selected);
		drawEnterButton(&mBData[selected]);
		break;
	case SDL_MOUSEBUTTONUP:
		//rightMouseDown = false;
		selected = NON_SELECTED;
		if (p_buttonCheck != NULL && mDragButton != NON_SELECTED)
			mBData[mDragButton].fDepressed = false;
		mDragButton = NON_SELECTED;
		break;
	}
}

//! Screen_Manager  checkSelectedButton is the selected button ok?
//! 20150426
void Screen_Manager::checkSelectedButton()
{
	bool overButton = false;
	short oldSelected = returnSelectedButton();
	bool gotOne = false;

	for (int pnum = 0;; ++pnum) 
	{
		ButtonData *ed = &mBData[pnum];

		if (ed->id == ENTER_END) break;

		if (ed->status == B_INVISIBLE) continue;
		if (ed->status == B_RED_INACTIVE) continue;
		if (ed->status == B_INACTIVE) continue;
		if (ed->status == B_DEPRESSED) continue;
		if (ed->fShowSelected) continue;

		int sx = ed->dx + mWindowButtonLoc[ed->buttonLoc].x;
		int sy = ed->dy + mWindowButtonLoc[ed->buttonLoc].y;

		overButton = (miX >= sx && miY >= sy && miX <= sx + ed->width && miY <= sy + ed->height) ? true : false;
		if (!overButton) continue;

		if (oldSelected == pnum) return;
		if (mDragButton != NON_SELECTED && mDragButton != pnum) return;

		if (oldSelected == NON_SELECTED || (!mBData[oldSelected].fDepressed || ed->fDepressed))
		{
			gotOne = true;
			setSelectedButton(pnum, 1);

			if (mDragButton == pnum)
				setDepressedButton(pnum);

			drawEnterButton(ed);
		}
	}
	if (!gotOne) 
	{
		setDepressedButton(NON_SELECTED);
		setSelectedButton(NON_SELECTED, 2);
	}
}

//! Screen_Manager  returnSelectedButton this is the selected button
//! 20150426
short Screen_Manager::returnSelectedButton(short *d_depressed)
{
	short selected = NON_SELECTED;

	if (d_depressed) *d_depressed = NON_SELECTED;

	for (int pnum = 0;; pnum++)
	{
		ButtonData *ed = &mBData[pnum];

		if (ed->id == ENTER_END) break;
		if (ed->status == B_RED_INACTIVE) continue;
		if (ed->status == B_INACTIVE) continue;
		if (ed->status == B_RED_INACTIVE) continue;
		if (ed->status == B_DEPRESSED) continue;
		if (ed->status == B_INVISIBLE) continue;

		if (ed->fSelected)
		{
			selected = pnum;
			if (d_depressed)	*d_depressed = ed->fDepressed;
			break;
		}
	}

	return selected;
}

//! Screen_Manager  drawEnterButton button drawing routine
//! 20150426
void Screen_Manager::drawEnterButton(ButtonData *p_ed, short d_surpressDirty)
{
	if (!p_ed->width || !p_ed->height) return;

	// now we draw everything
	switch (p_ed->id)
	{
	case ENTER_BUTTON:
		drawButton(p_ed);
		break;
	case ENTER_TAB_BUTTON:
		break;
	case ENTER_TXT_BUTTON:
		drawTxtButton(p_ed);
		break;
	}
}

//! Screen_Manager  drawTxtButton draw a ENTER_TXT_BUTTON button
//! 20150426
void Screen_Manager::drawTxtButton(ButtonData *p_ed)
{
	if (p_ed->status == B_INVISIBLE)	return;

	short sy, tx, ty;
	bool draw = true;
	SDL_Color color = cText;


	// setup the button graphics
	SDL_Rect to, from;

	if (p_ed->status == B_INACTIVE)						sy = p_ed->sy + (p_ed->height + 1) * 3, color = cGray;
	else if (p_ed->status == B_RED_INACTIVE)			sy = p_ed->sy + (p_ed->height + 1) * 4, color = cRed;
	else if (p_ed->status == B_DEPRESSED)				sy = p_ed->sy + (p_ed->height + 1) * 2, color = cLtGreen;
	else if (p_ed->fDepressed || p_ed->fShowSelected)	sy = p_ed->sy + (p_ed->height + 1) * 2, color = cLtGreen;
	else if (p_ed->fSelected)							sy = p_ed->sy + p_ed->height + 1, color = cGreen;
	else {
		//TM_TEXT_BUTTON
		from.x = p_ed->sx;
		from.y = p_ed->sy + (p_ed->height + 1) * 5;
		from.w = p_ed->width;
		from.h = p_ed->height;
		to.x = p_ed->dx;
		to.y = p_ed->dy;
		to.w = from.w;
		to.h = from.h;

		to.x += mWindowButtonLoc[p_ed->buttonLoc].x;
		to.y += mWindowButtonLoc[p_ed->buttonLoc].y;

		tx = to.x;
		ty = to.y;

		SDL_SetTextureAlphaMod(TM_TEXT_BUTTON, 96);
		Window::Draw(TM_TEXT_BUTTON, to, &from);
		SDL_SetTextureAlphaMod(TM_TEXT_BUTTON, 255);

		from.y = p_ed->sy + (p_ed->height + 1) * 4;
		Window::Draw(TM_TEXT_BUTTON, to, &from);
		draw = false;
	}

	if (draw)
	{
		//TM_TEXT_BUTTON
		from.x = p_ed->sx;
		from.y = sy;
		from.w = p_ed->width;
		from.h = p_ed->height;
		to.x = p_ed->dx;
		to.y = p_ed->dy;
		to.w = from.w;
		to.h = from.h;

		to.x += mWindowButtonLoc[p_ed->buttonLoc].x;
		to.y += mWindowButtonLoc[p_ed->buttonLoc].y;

		tx = to.x;
		ty = to.y;

		Window::Draw(TM_TEXT_BUTTON, to, &from);
	}
	SDL_Color sc = { color.r, color.g, color.b };
	//string buttonName = str;
	Window::printString(tx + 46, ty + 9, *p_ed->string, sc, 15, PRINT_DROP);
}

//! Screen_Manager  drawButton draw a ENTER_BUTTON button
//! 20150426
void Screen_Manager::drawButton(ButtonData *p_ed)
{
	short sy, sx = p_ed->sx;

	if (p_ed->status == B_INVISIBLE)
	{
		return;
	}

	if (p_ed->fDragged)									sy = p_ed->sy + (p_ed->height + 1) * 2;
	else if (p_ed->status == B_INACTIVE)				sy = p_ed->sy + (p_ed->height + 1) * 3;
	else if (p_ed->status == B_RED_INACTIVE)			sy = p_ed->sy + (p_ed->height + 1) * 4;
	else if (p_ed->status == B_DEPRESSED)				sy = p_ed->sy + (p_ed->height + 1) * 2;
	else if (p_ed->fDepressed || p_ed->fShowSelected)	sy = p_ed->sy + (p_ed->height + 1) * 2;
	else if (p_ed->fSelected)							sy = p_ed->sy + p_ed->height + 1;
	else												sy = p_ed->sy;

	if (sy < 0) return;

	SDL_Rect scrnRect = Window::Box();
	SDL_Rect s1 = { sx, sy, p_ed->width, p_ed->height };
	SDL_Rect d1 = { p_ed->dx, p_ed->dy, s1.w, s1.h };

	d1.x += mWindowButtonLoc[p_ed->buttonLoc].x;
	d1.y += mWindowButtonLoc[p_ed->buttonLoc].y;

	Window::Draw(mTexture, d1, &s1);
}

//! Screen_Manager  setDepressedButton set button has been depressed
//! 20150426
void Screen_Manager::setDepressedButton(short p_cnt)
{
	short pnum = 0;

	for (;; pnum++)
	{
		ButtonData *ed = &mBData[pnum];

		if (ed->id == ENTER_END) break;

		ed->fDepressed = false;

		if (pnum == p_cnt) {
			ed->fDepressed = true;

		}
	}
}

//! Screen_Manager  setSelectedButton set button has been selected
//! 20150426
void Screen_Manager::setSelectedButton(short p_cnt, short d_numx)
{
	short pnum = 0;

	for (;; pnum++) 
	{
		ButtonData *ed = &mBData[pnum];

		if (ed->id == ENTER_END) return;

		ed->fSelected = false;

		if (pnum == p_cnt) {
			ed->fSelected = true;
		}
	}
}

//! Screen_Manager  drawCurrentButtonsDisplay 
//! 20150426
void Screen_Manager::drawCurrentButtonsDisplay()
{
	if (p_drawScreen == NULL) return;

	checkSelectedButton();

	drawAllButtonData();
}

//! Screen_Manager  drawAllButtonData 
//! 20150426
void Screen_Manager::drawAllButtonData()
{
	short pnum = 0;


	for (;; pnum++) 
	{
		ButtonData *ed = &mBData[pnum];

		if (ed->id == ENTER_END)	break;
		if ((ed->status = (*p_buttonCheck)(ed->name)) == B_IGNORE) continue;

		if (ed->status == B_INVISIBLE) continue;

		drawEnterButton(ed, true);
	}
}

//! Screen_Manager  returnOverButton 
//! 20150426
short Screen_Manager::returnOverButton()
{
	short pnum = 0, height;

	for (;; pnum++)
	{
		ButtonData *ed = &mBData[pnum];

		if (ed->id == ENTER_END) break;
		if (ed->status == B_INVISIBLE) continue;

		height = ed->height;

		if (miX >= ed->dx && miY >= ed->dy && miX <= ed->dx + ed->width && miY <= ed->dy + height) {
			return pnum;
		}
	}

	return NON_SELECTED;
}

//! Screen_Manager  centreButtons 
//! 20150426
void Screen_Manager::centreButtons()
{
	SDL_Rect scrnRect = Window::Box();

	for (int pnum = 0;; ++pnum)
	{
		ButtonData *ed = &mBData[pnum];

		if (ed->id == ENTER_END) break;

		if (ed->buttonLoc != WB_CENTER) continue;

		ed->dx = scrnRect.w / 2 - ed->width / 2;
	}
}

// end of managerScreen.cpp