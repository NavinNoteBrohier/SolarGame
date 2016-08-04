#ifndef _MANAGER_SCREEN_H_
#define _MANAGER_SCREEN_H_

#include <string>

enum BUTTON_ID;
#define NON_SELECTED -1

enum BUTTON_STATE { B_IGNORE = -1, B_NORMAL = 0, B_INACTIVE, B_INVISIBLE, B_DEPRESSED, B_RED_INACTIVE, MAX_BUTTON_STATES, };

// type of UI elements allowed in the game
enum BUTTON_TYPE {
	ENTER_NULL = 0, ENTER_BUTTON = 5000, ENTER_TAB_BUTTON, ENTER_TXT_BUTTON, ENTER_PAD, ENTER_END
};
enum BUTTON_RETURN {	// return from 'hit button'
	BUTTON_DID_NOTHING, BUTTON_IS_ACTIVE, BUTTON_IS_DONE, BUTTON_IS_UPDATE, BUTTON_IS_NEW_ITEM, BUTTON_IS_NEXT_ITEM,
	BUTTON_IS_PREV_ITEM, BUTTON_IS_CURR_ITEM, BUTT_IS_DEL_ITEM, BUTTON_IS_FINISHED,
};

// where does the button 'rethink' its co-ordinates
//enum windowPosition { WB_NULL, WB_CENTER, WB_MAIN_INTER, WB_MAIN_SELECT, WB_MAIN_TOP_INFO, WB_MAIN_UNITS, WB_MAIN_STRAT, WB_MAIN_COMBINED, WB_MAIN_INTERDICT, WB_COMBAT_RESULT, WB_CONFIRM_DIALOG, WB_EDITOR, WB_MAX_LOC };
enum windowPosition { WB_NULL, WB_CENTER, WB_MAIN_TOP_INFO, WB_MAX_LOC };

extern SDL_Color cWhite, cRed, cYellow, cLtGreen, cGreen, cDkGreen, cCyan, cBlue, cPurple, cText, cBlack, cDkGray, cGray, cLtGray, cTransparent, cOrange, cBrown;

typedef unsigned short xUWORD;

// UI element structure
typedef struct {
	BUTTON_TYPE id;					// type of button item
	BUTTON_ID name;					// unique name of that button

	windowPosition buttonLoc;		// window that will be followed

	short dx;						// destination co-ordinates relative to the window position
	short dy;
	short width;
	short height;

	short sx;						// source co-ordinates
	short sy;

	std::string *string;			// pointer to string
	short setWidth;					// to get a set width string button

	unsigned char *numberX;			// pointer to number
	short minNum;					// min - max of number input (ENTER_NUMBER/ENTER_DROP_MENU)
	short maxNum;
	short maxStr;					// maximum length of string (ENTER_STRING) OR menutype (ENTER_DROP_MENU)

	short status;					// current status of button

									// flags for buttons
	bool	fSelected;				// 
	bool	fDepressed;				//
	bool	fShowSelected;			//
	bool	fDragged;				//

	short stored_dx;				// stored start location co-ordinates 
	short stored_dy;
}ButtonData;

class Screen_Manager {
public:
	Screen_Manager();				
	~Screen_Manager() {}			

	void checkSelectedButton();
	short returnSelectedButton(short *d_depressed = false);
	void drawEnterButton(ButtonData *p_ed, short d_surpressDirty = false);
	void setDepressedButton(short p_cnt);
	void setSelectedButton(short p_cnt, short d_num = 0);
	void depressCurrentButtons(bool shiftKey, SDL_Event e);
	void moveCurrentButtons(short shiftKey, SDL_Event e);
	bool releaseCurrentButtons(short shiftKey, SDL_Event e);
	void drawCurrentButtonsDisplay();
	void centreButtons();

	BUTTON_STATE(*p_buttonCheck)(BUTTON_ID p_buttonId);
	BUTTON_RETURN(*p_buttonHit)(BUTTON_ID p_buttonId, short shiftKey);
	void(*p_drawScreen)(SDL_Event *a_e);
	bool(*p_checkKeyboard)(SDL_Event *a_e);
	// pointer to the current button data
	ButtonData *mBData;
	SDL_Texture *mTexture;
private:
	void checkCurrentButtons(short shiftKey, SDL_Event e);			// check all current buttons
	void actOnActiveButton(ButtonData *p_ed, short p_shiftOn);
	void drawAllButtonData();
	void drawTxtButton(ButtonData *p_ed);
	void drawButton(ButtonData *p_ed);
	short returnOverButton();

	int miX;							// mouse coordinates
	int miY;
	SDL_Point mWindowButtonLoc[WB_MAX_LOC];
	int mDragButton;
	bool mLeftB;
	bool mRightB;
};

extern Screen_Manager gSM;

#endif // _MANAGER_SCREEN_H_