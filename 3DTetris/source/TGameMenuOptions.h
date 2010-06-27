#ifndef _TGAMEMENUOPTIONS_H_
#define _TGAMEMENUOPTIONS_H_
#include "tgamemenu.h"

class TGameMenuOptions :
	public TGameMenu
{
public:
	TGameMenuOptions(void);
	~TGameMenuOptions(void);
public:
	static TGameMenuOptions *getInst() { return &mGameMenuOptions; }
private:
	static TGameMenuOptions mGameMenuOptions;
};

#endif