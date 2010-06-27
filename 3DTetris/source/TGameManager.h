#ifndef _TGAMEMANAGER_H_
#define _TGAMEMANAGER_H_

#define WINDOW_TITLE "PuzzleGame"

#include <Ogre.h>
#include <OgreFrameListener.h>

#include <OIS.h>

#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>

class TGameState;

class TGameManager : 
	public Ogre::Singleton<TGameManager>, 
	public Ogre::FrameListener, 
	public OIS::KeyListener, 
	public OIS::MouseListener
{
public:
	TGameManager(void);
	~TGameManager(void);
public:
	static TGameManager& getSingleton(void);
	static TGameManager* getSingletonPtr(void);

	bool start(TGameState *state);
	void pushGameState(TGameState *state);
	void popGameState();
	void changeGameState(TGameState *state);
	MyGUI::OgrePlatform	*getGUIPlatform();
public:

	OIS::Keyboard			*mpKeyboard;
	OIS::Mouse				*mpMouse;
	Ogre::Viewport			*mpViewport;
	unsigned int			mWidth, mHeight, mDepth;
	int						mTop, mLeft;
	MyGUI::StaticTextPtr	mInfoHud;
protected:
	bool mouseMoved(const OIS::MouseEvent& e);
	bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id);

	bool keyPressed(const OIS::KeyEvent& e);
	bool keyReleased(const OIS::KeyEvent& e);

	bool frameStarted(const Ogre::FrameEvent &evt);
	bool frameEnded(const Ogre::FrameEvent &evt);
private:
	Ogre::Root				*mpRoot;
	OIS::InputManager		*mpInputManager;
	Ogre::RenderWindow		*mpWindow;
	MyGUI::Gui				*mpGUI;
	MyGUI::OgrePlatform		 *mpPlatform;
	size_t				mWndHandle;

	std::vector<TGameState*> mStateList;
private:
	void shutdown();
	void clearStates();

	void setupResourcePaths(Ogre::String configFile);
	void setupInput();
	bool initOgre();
	void setupGUI();
	void showInfoHUD();
};

#endif