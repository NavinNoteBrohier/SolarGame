#ifndef _MAIN_H_
#define _MAIN_H_
#include <string.h>

using namespace std;

class MainLoop {
public:
	MainLoop();					// contructor
	~MainLoop();				// destructor

	void setStartDirectory(char *p_str);
	string *getStartDirectory();
	Uint32 start_time;			// program starts at this point
	bool   quit_game;			// stop the game
private:
	string mStartDirectory;
};

extern MainLoop gML;

#endif //_MAIN_H_