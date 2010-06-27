#ifndef _TGAMEMENU_H_
#define _TGAMEMENU_H_

#include "TGameState.h"
#include <Ogre.h>
// Abstract class for game menu
class TGameMenu :
	public TGameState
{
protected:
	TGameMenu(void){};
protected:
	virtual	void enter() = 0;
	virtual void exit() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual bool keyPressed(const OIS::KeyEvent& e) { return true; }
	virtual bool keyReleased(const OIS::KeyEvent& e){ return true; }

	virtual bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id){ return true; }
	virtual bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id){ return true; }
	virtual bool mouseMoved(const OIS::MouseEvent& e){ return true; }

	virtual bool frameStarted(const Ogre::FrameEvent &evt){ return !mbShutdown; }
	virtual bool frameEnded(const Ogre::FrameEvent &evt){ return true; }

	virtual void setupGUI() = 0;
	
	void setupSceneMng();
	void setupCamera();
	void setupViewport();
	void preSetupGUI();
protected:
	static Ogre::SceneManager		*mpSceneMng;
	static Ogre::Camera				*mpCamera;
	static Ogre::Viewport			*mpViewport;
	
	static MyGUI::Gui				*mpGUI;
	//static MyGUI::OgrePlatform		*mpPlatform;
	static MyGUI::WindowPtr			mMenuWindow;
	bool							mbShutdown;
};

#endif