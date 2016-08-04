#ifndef _SDL_WINDOW_H_
#define _SDL_WINDOW_H_

#include <string>
using namespace std;

enum screenChoice { SCREEN_FULL, SCREEN_WINDOW, SCREEN_WINDOW_NO_BORDER };
enum printType { PRINT_NULL, PRINT_DROP, PRINT_KEYLINE };
enum printLoc { PRINT_LOC_NULL, PRINT_LOC_LEFT, PRINT_LOC_RIGHT, PRINT_LOC_CENTRE };

//What we think our window class should look like
class Window {
public:
	//Start SDL and TTF, create the window and renderer
	static int Init(std::string title = "Window");
	//Quit SDL and TTF
	static void Quit();
	//create a screenshot bmp
	static void screenShot();
	//Draw an SDL_Texture
	static void Draw(SDL_Texture *tex, SDL_Rect &dstRect, SDL_Rect *clip = NULL,
		float angle = 0.0, int xPivot = 0, int yPivot = 0,
		SDL_RendererFlip flip = SDL_FLIP_NONE);
	//Clear window
	static void Clear();
	//Present renderer
	static void Present();
	//Get the window's box
	static SDL_Rect Box();
	// needed for loading resources
	static SDL_Texture* Window::LoadThisImage(const std::string &file);
	// Generate a texture containing the message we want to display
	static SDL_Texture* RenderText(const std::string &message, const std::string &fontFile,
		SDL_Color color, int fontSize);

	// create a texture from a surface
	static SDL_Texture* createTexture(SDL_Surface *sur, bool p_streaming = false);
	// draw a filled rectangle on the screen
	static void rectangle(SDL_Rect *rt, SDL_Color *cl, bool fill = true, SDL_Color *cl1 = NULL);
	// draw a line on the screen
	static void lineColor(int x1, int y1, int x2, int y2, SDL_Color *cl);
	// minimize the window
	static void minimizeWindow();
	// toggle fullscreen / windowed mode in real time
	static int toggle_fullscreen();
	static SDL_bool IsFullScreen(SDL_Window *win);
	// is the window bordered?
	static bool isWindowBordered();
	// change bordered status
	static void setWindowBordered(bool toThis);
	// true type font printout function
	static void printString(short x, short y, string text, SDL_Color color, short fontSize = 15, printType type = PRINT_NULL, printLoc pl = PRINT_LOC_LEFT, std::string font = "ebrimabd.ttf");
	static void printStringShadow(short x, short y, string text, SDL_Color color, short fontSize = 15, printType type = PRINT_NULL, printLoc pl = PRINT_LOC_LEFT, string font = "ebrimabd.ttf");
	// set window size and type
	static void setWindowSize(SDL_Rect p_toThis, screenChoice p_thisType);
	// draws a line
	static void drawLine(int p_x1, int p_y1, int p_x2, int p_y2);

	static void setDrawColor(Uint8 p_r, Uint8 p_g, Uint8 p_b, Uint8 p_a);
	static void DrawRect(const SDL_Rect* rect);
	static void FillRect(const SDL_Rect* rect, SDL_Color p_color);
	static int initCursor(int p_value);

private:
	static SDL_Window       *mWindow;
	static SDL_Renderer		*mRenderer;
	static screenChoice		mWindowType;
	static SDL_Rect			mBox;					// actual screen size
	static bool				mBordered;
	static SDL_Rect			mScreenSize;			// requested size from the options system
													// the screen will be forced to this setup
	static SDL_Rect			mToThis;
	static screenChoice		mThisType;
};

#endif	// _SDL_WINDOW_H_