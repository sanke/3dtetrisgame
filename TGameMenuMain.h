#ifndef _TGAMEMENUMAIN_H_
#define _TGAMEMENUMAIN_H_
#include "TGameMenu.h"

class TGameMenuMain :
	public TGameMenu
{
public:
	TGameMenuMain(void);
	~TGameMenuMain(void);
public:
	static TGameMenuMain *getInst() { return &mGameMenuMain; }
protected:
	void enter();
	void exit();
	void pause();
	void resume();
private:
	static TGameMenuMain mGameMenuMain;
private:
	void setupGUI();
	void buttonStartClick(MyGUI::WidgetPtr _widget);
	void buttonQuitClick(MyGUI::WidgetPtr _widget);
};

#endif