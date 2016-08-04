////////////////////////////////////////////////////////////////////////////////////////////////////////////
////																									////
////		Name:		sdlWindow.cpp : SDL2 Template										     		////
////																									////
////		Purpose:	Window routines in SDL2											                ////
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

#include "main.h"
#include "sdlWindow.h"
#include "sdlResources.h"

#include "../Screens/screenManager.h"
///#include "../Maths/Vector2.h"

using namespace std;

SDL_Rect		 Window::mBox;
SDL_Rect		 Window::mScreenSize;
screenChoice	 Window::mWindowType;
bool			 Window::mBordered;
SDL_Window		*Window::mWindow = nullptr;
SDL_Renderer	*Window::mRenderer = nullptr;
SDL_Rect		 Window::mToThis = { 0 };
screenChoice	 Window::mThisType = SCREEN_WINDOW;


struct window_data
{
	SDL_Rect bounds;
	SDL_Window *window;
};
static int screenDisplays;
vector< window_data > screens;

//! Window : Init - initialize the window
//! 20150426
int Window::Init(string title) {
	//initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)	return 1; // error 1
	if (TTF_Init() == -1)						return 2; // error 2

	screenDisplays = SDL_GetNumVideoDisplays();

	for (int i = 0; i < screenDisplays; ++i) {
		window_data tempData;
		SDL_GetDisplayBounds(i, &(tempData.bounds));
		screens.push_back(tempData);
	}

	// set the default values
	if (mToThis.w && mToThis.h) {
		mWindowType = mThisType;
		mScreenSize.w = mBox.w = mToThis.w;
		mScreenSize.h = mBox.h = mToThis.h;
		if (!mToThis.x && !mToThis.y) {
			mToThis.x = (screens[0].bounds.w - mToThis.w) / 2;
			mToThis.y = (screens[0].bounds.h - mToThis.h) / 2;
		}
		mScreenSize.x = mBox.x = mToThis.x;
		mScreenSize.y = mBox.y = mToThis.y;
	} else {
		mWindowType = SCREEN_WINDOW;
		mBox.x = 30;
		mBox.y = 60;
		mBox.w = screens[0].bounds.w - 60;
		mBox.h = screens[0].bounds.h - 120;
		mScreenSize.x = 30;
		mScreenSize.y = 60;
		mScreenSize.w = screens[0].bounds.w - 60;
		mScreenSize.h = screens[0].bounds.h - 120;
		mBordered = true;
	}

	// get the screen width and height
	SDL_DisplayMode mode;
	SDL_GetDesktopDisplayMode(0, &mode);

	//Setup our window size
	if (mWindowType == SCREEN_FULL) {
		mBox.x = 0;
		mBox.y = 0;
		mBox.w = screens[0].bounds.w;
		mBox.h = screens[0].bounds.h;
	} else {
		if (!mScreenSize.w || !mScreenSize.h) {
			mBox.x = 30;
			mBox.y = 30;
			mScreenSize.w = screens[0].bounds.w - 60;
			mScreenSize.h = screens[0].bounds.h - 140;
		}

		if (mScreenSize.w == screens[0].bounds.w)
			mScreenSize.w -= 40;
		if (mScreenSize.h == screens[0].bounds.h)
			mScreenSize.h -= 60;
		if (!mScreenSize.x) mScreenSize.x = 30;
		if (!mScreenSize.y) mScreenSize.y = 10;
		mBox.w = mScreenSize.w;
		mBox.h = mScreenSize.h;
	}

	// some defines to note here
	//SDL_WINDOW_FULLSCREEN = 0x00000001,         /**< fullscreen window */
	//SDL_WINDOW_OPENGL = 0x00000002,             /**< window usable with OpenGL context */
	//SDL_WINDOW_SHOWN = 0x00000004,              /**< window is visible */
	//SDL_WINDOW_HIDDEN = 0x00000008,             /**< window is not visible */
	//SDL_WINDOW_BORDERLESS = 0x00000010,         /**< no window decoration */
	//SDL_WINDOW_RESIZABLE = 0x00000020,          /**< window can be resized */
	//SDL_WINDOW_MINIMIZED = 0x00000040,          /**< window is minimized */
	//SDL_WINDOW_MAXIMIZED = 0x00000080,          /**< window is maximized */
	//SDL_WINDOW_INPUT_GRABBED = 0x00000100,      /**< window has grabbed input focus */
	//SDL_WINDOW_INPUT_FOCUS = 0x00000200,        /**< window has input focus */
	//SDL_WINDOW_MOUSE_FOCUS = 0x00000400,        /**< window has mouse focus */
	//SDL_WINDOW_FULLSCREEN_DESKTOP = ( SDL_WINDOW_FULLSCREEN | 0x00001000 ),

	switch (mWindowType) {
	case SCREEN_FULL:
		mWindow = SDL_CreateWindow(title.c_str(), (mode.w - mBox.w) / 2, (mode.h - mBox.h) / 2, mBox.w, mBox.h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
		break;
	case SCREEN_WINDOW:
		if (!mScreenSize.x) mScreenSize.x = (mode.w - mBox.w) / 2;
		if (!mScreenSize.y) mScreenSize.y = (mode.h - mBox.h) / 2;

		mWindow = SDL_CreateWindow(title.c_str(), mScreenSize.x, mScreenSize.y, mBox.w, mBox.h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		setWindowBordered(true);
		break;
	case SCREEN_WINDOW_NO_BORDER:
		if (!mScreenSize.x) mScreenSize.x = (mode.w - mBox.w) / 2;
		if (!mScreenSize.y) mScreenSize.y = (mode.h - mBox.h) / 2;

		mWindow = SDL_CreateWindow(title.c_str(), mScreenSize.x, mScreenSize.y, mBox.w, mBox.h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS);
		break;
	default:
		mWindow = SDL_CreateWindow(title.c_str(), (mode.w - mBox.w) / 2, (mode.h - mBox.h) / 2, mBox.w, mBox.h, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
		break;
	}


	//Make sure it created ok
	if (mWindow == nullptr)	return 3; // error 3

	//Create the renderer
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//Make sure it created ok
	if (mRenderer == nullptr)	return 4; // error 4

	return 0;
}


//! Window : isWindowBordered - has the window a border
//! 20150426
bool Window::isWindowBordered()
{
	return mBordered;
}

//! Window : setWindowBordered - set the window border on / off
//! 20150426
void Window::setWindowBordered(bool toThis)
{
	if (toThis == mBordered) return;
	mBordered = toThis;
	SDL_SetWindowBordered(mWindow, mBordered ? SDL_TRUE : SDL_FALSE);
}

#include <direct.h>
#include <sys/stat.h>
#include <fstream>

//! Window : screenShot - take a screen shot 
//! 20150426
void Window::screenShot() {
	char tempStr[256];
	string folder = *gML.getStartDirectory() + "/Screenshots";

	if (_mkdir(folder.c_str()) != 0) {
		// error not made
	}

	bool gotFile = false;
	for (short k = 0; k<100; k++) {
		sprintf_s(tempStr, "%s/Screenshot%02d.bmp", folder.c_str(), k);
		// does a screenshot exist
		ifstream ifile(tempStr);
		if (ifile.is_open()) {
			ifile.close();
			continue;
		}

		// this is ok to save
		gotFile = true;
		break;
	}
	// always use location '0' if there are no more
	if (!gotFile) {
		sprintf_s(tempStr, "%s/Screenshot%02d.bmp", folder.c_str(), 0);
	}
	// make a sound of a camera
	gRes.playSound(WAVE_CAMERA);

	SDL_Rect scrnRect = Window::Box();
	SDL_Surface *sshot = SDL_CreateRGBSurface(0, scrnRect.w, scrnRect.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(mRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
	SDL_SaveBMP(sshot, tempStr);
	SDL_FreeSurface(sshot);
}


//! Window : Quit - get rid of all the SDL stuff 
//! 20150426
void Window::Quit() {
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();

	mWindow = nullptr;
	mRenderer = nullptr;
}

/**
*  Draw a SDL_Texture to the screen at dstRect with various other options
*  @param tex The SDL_Texture to draw
*  @param dstRect The destination position and width/height to draw the texture with
*  @param clip The clip to apply to the image, if desired
*  @param angle The rotation angle to apply to the texture, default is 0
*  @param xPivot The x coordinate of the pivot, relative to (0, 0) being center of dstRect
*  @param yPivot The y coordinate of the pivot, relative to (0, 0) being center of dstRect
*  @param flip The flip to apply to the image, default is none
*/
//! Window : Draw - this is the one to play with 
//! 20150426
void Window::Draw(SDL_Texture *tex, SDL_Rect &dstRect, SDL_Rect *clip, float angle, int xPivot, int yPivot, SDL_RendererFlip flip)
{
	//Convert pivot pos from relative to object's center to screen space
	xPivot += dstRect.w / 2;
	yPivot += dstRect.h / 2;
	//SDL expects an SDL_Point as the pivot location
	SDL_Point pivot = { xPivot, yPivot };

	//Draw the texture
	SDL_RenderCopyEx(mRenderer, tex, clip, &dstRect, angle, &pivot, flip);
}

//! Window : Clear - clear window so that it can be redrawn
//! 20150426
void Window::Clear() {
	SDL_RenderClear(mRenderer);
}

//! Window : Present - show the newly drawn window
//! 20150426
void Window::Present() {
	SDL_RenderPresent(mRenderer);
}

//! Window : Box - what is the size of the currently displayed window
//! 20150426
SDL_Rect Window::Box() {
	//Update mBox to match the current window size
	SDL_GetWindowSize(mWindow, &mBox.w, &mBox.h);
	return mBox;
}

//! Window : RenderText - when ever printing text to the screen you will need this step (printString uses it)
//! 20150426
SDL_Texture* Window::RenderText(const std::string &message, const std::string &fontFile,
	SDL_Color color, int fontSize) {
	//Open the font
	TTF_Font *font = nullptr;
	font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr) return nullptr;

	//Render the message to an SDL_Surface, as that's what TTF_RenderText_X returns
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer, surf);
	//Clean up unneeded stuff
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);

	return texture;
}

//! Window : LoadThisImage - needed for loading resources
//! 20150426
SDL_Texture* Window::LoadThisImage(const std::string &file) {
	SDL_Texture* tex = nullptr;
	tex = IMG_LoadTexture(mRenderer, file.c_str());
	if (tex == nullptr) {
		char tempStr[400];
		sprintf_s(tempStr, "Failed to load image: %s %s", file.c_str(), IMG_GetError());
		return nullptr;
	}
	//throw std::runtime_error("Failed to load image: " + file + IMG_GetError());    
	return tex;
}

//! Window : createTexture 
//! 20150426
SDL_Texture* Window::createTexture(SDL_Surface *sur, bool p_streaming) {
	SDL_Texture* tex = nullptr;

	if (p_streaming) {
		SDL_Texture* tx = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, sur->w, sur->h);
		if (tx == nullptr) return nullptr;
		return tex;
	}

	tex = SDL_CreateTextureFromSurface(mRenderer, sur);
	if (tex == nullptr) {
		//char tempStr[400];
		//sprintf_s(tempStr, "Failed to load image: %s %s",file.c_str(), IMG_GetError());
		return nullptr;
	}
	//throw std::runtime_error("Failed to load image: " + file + IMG_GetError());  
	//SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 640, 480);
	return tex;
}

//! Window : rectangle - draw a rectangle on the screen 
//! 20150426
void Window::rectangle(SDL_Rect *p_rt, SDL_Color *p_cl, bool p_fill, SDL_Color *p_cl1)
{
	if (p_fill) {
		SDL_BlendMode blendMode;
		SDL_GetRenderDrawBlendMode(mRenderer, &blendMode);

		SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
		//SDL_SetTextureAlphaMod(mmm);
		SDL_SetRenderDrawColor(mRenderer, p_cl->r, p_cl->g, p_cl->b, p_cl->a);
		SDL_RenderFillRect(mRenderer, p_rt);

	}
	else {
		SDL_SetRenderDrawColor(mRenderer, p_cl->r, p_cl->g, p_cl->b, p_cl->a);
		if (p_cl1 != NULL) {
			SDL_RenderDrawLine(mRenderer, p_rt->x, p_rt->y, p_rt->x, p_rt->y + p_rt->h);
			SDL_RenderDrawLine(mRenderer, p_rt->x, p_rt->y, p_rt->x + p_rt->w, p_rt->y);

			SDL_SetRenderDrawColor(mRenderer, p_cl1->r, p_cl1->g, p_cl1->b, p_cl1->a);
			SDL_RenderDrawLine(mRenderer, p_rt->x + p_rt->w, p_rt->y + 1, p_rt->x + p_rt->w, p_rt->y + p_rt->h);
			SDL_RenderDrawLine(mRenderer, p_rt->x + 1, p_rt->y + p_rt->h, p_rt->x + p_rt->w, p_rt->y + p_rt->h);
		}
		else {
			SDL_RenderDrawRect(mRenderer, p_rt);
		}

	}
}

//! Window : lineColor - draw a line on the screen in color
//! 20150426
void Window::lineColor(int p_x1, int p_y1, int p_x2, int p_y2, SDL_Color *p_cl)
{
	SDL_SetRenderDrawColor(mRenderer, p_cl->r, p_cl->g, p_cl->b, p_cl->a);
	SDL_RenderDrawLine(mRenderer, p_x1, p_y1, p_x2, p_y2);
}

//! Window : minimizeWindow - minimize the window
//! 20150426
void Window::minimizeWindow()
{
	SDL_MinimizeWindow(mWindow);
}

//! Window : IsFullScreen - true of false
//! 20150426
SDL_bool Window::IsFullScreen(SDL_Window *win)
{
	Uint32 flags = SDL_GetWindowFlags(win);

	if (flags & SDL_WINDOW_FULLSCREEN) return SDL_TRUE; // return SDL_TRUE if fullscreen 

	return SDL_FALSE; // Return SDL_FALSE if windowed 
}

/// Toggles On/Off FullScreen 
/// @returns -1 on error, 1 on Set fullscreen successful, 0 on Set Windowed successful 
int Window::toggle_fullscreen()
{
	if (IsFullScreen(mWindow))
	{
		// Swith to WINDOWED mode 
		if (SDL_SetWindowFullscreen(mWindow, SDL_FALSE) < 0)
		{
			std::cout << "Setting windowed failed : " << SDL_GetError() << std::endl;
			return -1;
		}
		// and correct the bordered status
		SDL_SetWindowBordered(Window::mWindow, mBordered ? SDL_TRUE : SDL_FALSE);
		return 0;
	}

	// Swith to FULLSCREEN mode 
	if (SDL_SetWindowFullscreen(mWindow, SDL_TRUE) < 0)
	{
		std::cout << "Setting fullscreen failed : " << SDL_GetError() << std::endl;
		return -1;
	}

	return 1;
}

//! Window : setWindowSize - records what you want to do before init() does it so can be called by options etc.
//! 20150426
void Window::setWindowSize(SDL_Rect p_toThis, screenChoice p_thisType)
{
	mToThis = p_toThis;
	mThisType = p_thisType;
}

void Window::printStringShadow(short x, short y, string text, SDL_Color color, short fontSize, printType type, printLoc pl, string font)
{
	Window::printString(x + 4, y, text, cBlack, fontSize, type, pl, font);
	Window::printString(x, y + 4, text, cBlack, fontSize, type, pl, font);
	Window::printString(x + 4, y + 4, text, cBlack, fontSize, type, pl, font);
	Window::printString(x, y, text, color, fontSize, type, pl, font);
}

//! Window : printString - draw a text string on the screen
//! 20150426
void Window::printString(short x, short y, string text, SDL_Color color, short fontSize, printType type, printLoc pl, string font)
{
	SDL_Texture *mess;
	SDL_Rect msgBox;
	string fontFile = font;
	short kx[] = { -1, 0, +1, -1, +1 };
	short ky[] = { -1, +1, 0, 0, +1 };
	short ax = 0, ay = 0;

	// nothing to print
	if (text == "") return;

	if (type == PRINT_NULL) {
		mess = RenderText(text, fontFile, color, fontSize);
	}
	else {
		mess = RenderText(text, fontFile, cBlack, fontSize);
	}
	// error
	if (mess == nullptr) {
		return;
	}
	SDL_QueryTexture(mess, NULL, NULL, &msgBox.w, &msgBox.h);

	switch (pl) {
	case PRINT_LOC_CENTRE:
		ax = -(msgBox.w / 2);
		break;
	case PRINT_LOC_RIGHT:
		ax = -msgBox.w;
		break;
	case PRINT_LOC_LEFT:
	case PRINT_NULL:
		break;
	}

	switch (type) {
	case PRINT_DROP:
		//Setup msg dstRect
		msgBox.x = x + 1 + ax;
		msgBox.y = y + 1;
		//Query w & h from texture
		SDL_QueryTexture(mess, NULL, NULL, &msgBox.w, &msgBox.h);
		Draw(mess, msgBox);
		SDL_DestroyTexture(mess);
		mess = RenderText(text, fontFile, color, fontSize);
		if (mess == nullptr) return;
		break;
	case PRINT_KEYLINE:
		for (short k = 0; k<5; ++k) {
			msgBox.x = x + kx[k] + ax;
			msgBox.y = y + ky[k];
			Draw(mess, msgBox);
		}
		SDL_DestroyTexture(mess);
		mess = RenderText(text, fontFile, color, fontSize);
		if (mess == nullptr) return;
		break;
	}
	// draw the actual text
	msgBox.x = x + ax;
	msgBox.y = y;
	//Query w & h from texture
	SDL_QueryTexture(mess, NULL, NULL, &msgBox.w, &msgBox.h);

	Draw(mess, msgBox);
	SDL_DestroyTexture(mess);
}

//! Window : drawLine - draw a text string on the screen
//! 20150426
void Window::drawLine(int p_x1, int p_y1, int p_x2, int p_y2)
{
	int error = SDL_RenderDrawLine(mRenderer, p_x1, p_y1, p_x2, p_y2);
}

//! Window : setDrawColor - set the draw color for lines, rectangles etc.
//! 20150426
void Window::setDrawColor(Uint8 p_r, Uint8 p_g, Uint8 p_b, Uint8 p_a)
{
	int error = SDL_SetRenderDrawColor(mRenderer, p_r, p_g, p_b, p_a);
}

//! Window : DrawRect - draw a rectangle on the screen
//! 20150426
void Window::DrawRect(const SDL_Rect* rect)
{
	SDL_RenderDrawRect(mRenderer, rect);
}

//! Window : FillRect - draw a filled in rectangle on the screen
//! 20150426
void Window::FillRect(const SDL_Rect* rect, SDL_Color p_color)
{
	setDrawColor(p_color.r, p_color.g, p_color.b, p_color.a);
	SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
	SDL_RenderFillRect(mRenderer, rect);
}

//! Window : initCursor 1 to show the cursor, 0 to hide it, -1 to query the current state
//! 20150426
int Window::initCursor(int p_value)
{
	// cursor control - 0 no visible cursor
	return SDL_ShowCursor(p_value);
}

// end of Window.cpp