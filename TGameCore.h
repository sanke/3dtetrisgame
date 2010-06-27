#ifndef _TGAMECORE_H_
#define _TGAMECORE_H_

#define _Log (LogManager::getSingleton().getDefaultLog())

#define WINDOW_TITLE "PuzzleGame"


#include <Ogre.h>
//#include "OgreFrameListener.h"

#include <OIS/OIS.h>

#include <CEGUI/CEGUI.h>
#include <OgreCEGUIRenderer.h>



using namespace Ogre;
using namespace OIS;

class TGameCore : public FrameListener, public KeyListener, 
	public MouseListener, public Singleton<TGameCore>
{
public:
	TGameCore(void) : mWndHandle(0), camSpeed(1000) {};
	~TGameCore(void) {};
	
	bool Start();
	bool Initialize();

	//static TGameCore& getSingleton(void);
	//static TGameCore* getSingletonPtr(void);
private:

	const int			camSpeed;

	Root				*mpRoot;

	RenderWindow		*mpMainWnd;
	
	SceneManager		*mpMainSceneMng;

	Camera				*mpCamera;

	Viewport			*mpViewport;

	CEGUI::System		*mpGUISystem;
	CEGUI::Window		*FPS;
	CEGUI::OgreCEGUIRenderer	*mpGUIRenderer;


	size_t				mWndHandle;

	unsigned int mWidth, mHeight, mDepth;
	int mTop, mLeft;

	void			CreateMainScene();

	InputManager	*mpInputManager;
	Keyboard		*mpKeyboard;
	Mouse			*mpMouse;

	bool LoadConfig(String configFile);
	
	bool SetupCamera();
	bool SetupBoard();
	bool SetupGUI();

	void Shutdown();
	
	void UpdateStatWnd();

	CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);
protected:
	bool mouseMoved(const MouseEvent& e);
	bool mousePressed(const MouseEvent& e, OIS::MouseButtonID id);
	bool mouseReleased(const MouseEvent& e, OIS::MouseButtonID id);

	bool keyPressed(const KeyEvent& e);
	bool keyReleased(const KeyEvent& e);

	bool frameStarted(const FrameEvent &evt);
	bool frameRenderingQueued(const FrameEvent &evt);
	bool frameEnded(const FrameEvent &evt);
};

#endif