#ifndef _SDL_RESOURCES_H_
#define _SDL_RESOURCES_H_

enum LOAD_TYPE { LOAD_SURFACE, LOAD_TEXTURE, LOAD_CURSOR, LOAD_FONT, LOAD_SOUND, LOAD_MUSIC, LOAD_END = -1 };
enum ERROR_LOADING { ERROR_SURFACE = 5100, ERROR_TEXTURE = 5200, ERROR_CURSOR = 5300, ERROR_CREATE = 5400, ERROR_SOUNDS = 5500, ERROR_MUSIC = 5600, ERROR_MUSIC_SETUP = 5650, ERROR_CURSORS = 5599 };

extern SDL_Surface *SM_TANKS, *SM_CURSOR;
extern SDL_Texture *TM_TANKS, *TM_HANGPAPER,*TM_WALLPAPER, *TM_GOTONEXT, *TM_HANGMAN, *TM_SMALLRECT, *TM_TEXT_BUTTON, *TM_HANG_BUTTONS, 
*TM_FIRE,*TM_FARM, *TM_TRACK, *TM_SPRITEPAPER, *TM_SPRITECHARACTER, *TM_SHIP, *TM_WAKE, *TM_NODE, *TM_NODEEND, *TM_NODEENEMY, 
*TM_NODEASTEROID,*TM_NODEEMPTY,*TM_PODSHIP,*TM_SUN, *TM_PLANETONE, *TM_BASESTATION,*TM_MOONONE,*TM_DEPLETED;


extern Mix_Chunk *WAVE_SCRATCH, *WAVE_HIGH, *WAVE_MEDIUM, *WAVE_LOW, *WAVE_CAMERA;
extern Mix_Music *WAVE_MUSIC;

typedef struct {
	LOAD_TYPE type;
	SDL_Surface **sur;
	string name;
	ERROR_LOADING errorCode;
} loadSurface;

typedef struct {
	LOAD_TYPE type;
	SDL_Texture **tex;
	string name;
	ERROR_LOADING errorCode;
} loadTexture;

typedef struct {
	LOAD_TYPE type;
	Mix_Chunk **snd;
	string name;
	ERROR_LOADING errorCode;
} loadSounds;

typedef struct {
	LOAD_TYPE type;
	Mix_Music **mus;
	string name;
	ERROR_LOADING errorCode;
} loadMusic;

enum CursorType {
	CURS_NULL, CURS_ARROW, CURS_POINTER, CURS_SELECT, CURS_MAX
};

typedef struct {
	CursorType name;
	int cursorNumber;	// number of cursor and index into the cursor[x] files
	int maxFrames;		// max number of frames in this cursor

	int cWidth;			// width 
	int cHeight;		// height
	int hotX;			// hotspot
	int hotY;			// hotspot

	int sourceX;		// x co-ord of cursors bitmap
	int sourceY;		// y co-ord of cursors bitmap

	unsigned int speed;	// in miliseconds
	bool loaded;		// is it loaded
	SDL_Cursor *cursor;	// the cursor
} loadCursor;


class Resources {
public:
	Resources();							// constructor
	~Resources();							// deconstructor
	int loadResources();					// load all
	void unLoadResources();					// uload all
	void setCursor(CursorType p_type);
	void UpdateCursor();
	bool isSoundOn() { return soundOn; }
	void playSound(Mix_Chunk *p_sound);
	void playMusic(Mix_Music *p_sound);
private:
	void setCursorNumber(int  p_number);

	bool soundOn;
	int mCursor_time;
	int mCursorNumber;
	CursorType mCurrentCursorType;
};

extern Resources gRes;
extern SDL_Color GetPixelOne(SDL_Surface* pSurface, int x, int y);

#endif // _SDL_RESOURCES_H_
