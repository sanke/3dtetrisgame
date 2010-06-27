#ifndef _TGAMEPLAY_H_
#define _TGAMEPLAY_H_

#include "TGameState.h"

#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>

#define PLAY_SCENE "PLAY_SCENE"
#define PLAY_CAMERA "PLAY_CAMERA"


class TGamePlay :
	public TGameState
{
public:
	TGamePlay(void);
	~TGamePlay(void);
public:
	static TGamePlay *getInst() { return &mGamePlay; }
protected:
	void enter();
	void exit();
	void pause();
	void resume();

	bool keyPressed(const OIS::KeyEvent& e);
	bool keyReleased(const OIS::KeyEvent& e);

	bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id);
	bool mouseMoved(const OIS::MouseEvent& e);

	bool frameStarted(const Ogre::FrameEvent &evt);
	bool frameEnded(const Ogre::FrameEvent &evt);
private:
	void setupSceneMng();
	void setupCamera();
	void setupViewport();
	void setupGUI();
	
	void updateCamera(const OIS::MouseEvent &e);
private:
	static TGamePlay		mGamePlay;

	Ogre::SceneManager		*mpSceneMng;
	Ogre::Camera			*mpCamera;
	Ogre::SceneNode			*mpCameraNode, *mpCameraParent;
	Ogre::Viewport			*mpViewport;
	MyGUI::Gui				*mpGUI;

	Ogre::Radian			mAngle;

	bool					mbShutdown;
};

#endif