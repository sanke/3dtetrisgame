#include "TGameManager.h"
#include "TGameState.h"


using namespace Ogre;


template<> TGameManager* Singleton<TGameManager>::ms_Singleton = 0;

//=====================================================================
TGameManager::TGameManager(void)
{
	mpRoot = 0;
	mpInputManager = 0;
	mpKeyboard = 0;
	mpMouse = 0;
}
//=====================================================================
TGameManager::~TGameManager(void)
{
	// TODO
}
//=====================================================================
void TGameManager::setupResourcePaths(String configFile)
{
	ConfigFile config;

	config.load(configFile);

	ConfigFile::SectionIterator iter = config.getSectionIterator();

	String secName, typeName, archName;
	while (iter.hasMoreElements())
	{
		secName = iter.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = iter.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}
//=====================================================================
bool TGameManager::initOgre()
{
	mpRoot = new Root();

	setupResourcePaths("config.cfg");
	
	if(!mpRoot->restoreConfig())
	{
		if(!mpRoot->showConfigDialog())
		{
			delete mpRoot;
			return false;
		}
	}

	mpWindow = mpRoot->initialise(true, WINDOW_TITLE);
	
	mpViewport = mpWindow->addViewport(0);

	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	
	mpRoot->createSceneManager(ST_GENERIC, "PlayScene");

	setupInput();

	mpRoot->addFrameListener(this);
	
	setupGUI();

	return true;
}
//=====================================================================


//=====================================================================
void TGameManager::setupInput()
{
	OIS::ParamList OISParams;

	mpWindow->getCustomAttribute("WINDOW", &mWndHandle);
	mpWindow->getMetrics(mWidth, mHeight, mDepth, mLeft, mTop);

	std::ostringstream strWnd;

	strWnd << mWndHandle;

	OISParams.insert(std::make_pair(std::string("WINDOW"), strWnd.str()));

	mpInputManager = OIS::InputManager::createInputSystem(OISParams);
	
	mpKeyboard = static_cast<OIS::Keyboard*>(mpInputManager->createInputObject(OIS::OISKeyboard, true));
	mpMouse = static_cast<OIS::Mouse*>(mpInputManager->createInputObject(OIS::OISMouse,true));

	mpKeyboard->setEventCallback(this);

	mpMouse->setEventCallback(this);

	if (mpMouse) {
      const OIS::MouseState &ms = mpMouse->getMouseState();
      ms.width = (int)mWidth;
      ms.height = (int)mHeight;
    }
}
//=====================================================================
bool TGameManager::frameStarted(const FrameEvent &evt)
{
	//mpGUI->injectFrameEntered(evt.timeSinceLastFrame);

	showInfoHUD();

	if(mpKeyboard)
		mpKeyboard->capture();
	if(mpMouse)
		mpMouse->capture();
	
	return mStateList.back()->frameStarted(evt);
}
//=====================================================================
bool TGameManager::frameEnded(const FrameEvent &evt)
{

	return mStateList.back()->frameEnded(evt);
}
//=====================================================================
bool TGameManager::keyPressed(const OIS::KeyEvent &e)
{
	mpGUI->injectKeyPress(MyGUI::KeyCode::Enum(e.key));

	return mStateList.back()->keyPressed(e);
}
//=====================================================================
bool TGameManager::keyReleased(const OIS::KeyEvent &e)
{
	mpGUI->injectKeyRelease(MyGUI::KeyCode::Enum(e.key));

	return mStateList.back()->keyReleased(e);
}
//=====================================================================
bool TGameManager::mouseMoved(const OIS::MouseEvent &e)
{
	mpGUI->injectMouseMove(e.state.X.abs,e.state.Y.abs,e.state.Z.abs);

	return mStateList.back()->mouseMoved(e);
}
//=====================================================================
bool TGameManager::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	mpGUI->injectMousePress(e.state.X.abs, e.state.Y.abs, MyGUI::MouseButton::Enum(id));

	return mStateList.back()->mousePressed(e,id);
}
//=====================================================================
bool TGameManager::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	mpGUI->injectMouseRelease(e.state.X.abs, e.state.Y.abs,MyGUI::MouseButton::Enum(id));

	return mStateList.back()->mouseReleased(e,id);
}
//=====================================================================
void TGameManager::pushGameState(TGameState *state)
{
	if(!mStateList.empty())
		mStateList.back()->pause();

	mStateList.push_back(state);
	mStateList.back()->enter();
}
//=====================================================================
void TGameManager::popGameState()
{
	if(!mStateList.empty())
	{
		mStateList.back()->exit();
		mStateList.pop_back();
	}
	if(!mStateList.empty())
		mStateList.back()->resume();
}
//=====================================================================
void TGameManager::changeGameState(TGameState *state)
{
	//mpRoot->getSceneManager("PLAY_SCENE")->clearScene();
	if(!mStateList.empty())
	{
		mStateList.back()->exit();
		mStateList.pop_back();
	}
	
	mStateList.push_back(state);
	mStateList.back()->enter();
}
//=====================================================================
bool TGameManager::start(TGameState *state)
{
	if(!initOgre())
		return false;

	changeGameState(state);
	mpRoot->startRendering();

	shutdown();

	return true;
}
//=====================================================================
TGameManager *TGameManager::getSingletonPtr()
{
	return ms_Singleton;
}
//=====================================================================
TGameManager &TGameManager::getSingleton()
{
	assert(ms_Singleton);
	return *ms_Singleton;
}
//=====================================================================
// Shuts down the game
void TGameManager::shutdown()
{
	clearStates();
	//solves mygui shutdown error
	mpRoot->createSceneManager(ST_EXTERIOR_CLOSE);
	mpGUI->shutdown();
	delete mpGUI;
	mpRoot->shutdown();
	delete mpRoot;
	mpInputManager->destroyInputObject(mpKeyboard);
	mpInputManager->destroyInputObject(mpMouse);
	OIS::InputManager::destroyInputSystem(mpInputManager);
}
//=====================================================================
void TGameManager::clearStates()
{
	while(!mStateList.empty())
	{
		mStateList.back()->exit();
		mStateList.pop_back();
	}
}
//=====================================================================
void TGameManager::setupGUI()
{
	mpPlatform = new MyGUI::OgrePlatform();
	mpPlatform->initialise(mpRoot->getAutoCreatedWindow(), mpRoot->getSceneManager("PlayScene"));
	
	mpGUI = new MyGUI::Gui();
	mpGUI->initialise();
}
//=====================================================================
void TGameManager::showInfoHUD()
{
	//static String currFps = "Current FPS: ";
  /*  static String avgFps = "\nAverage FPS: ";
    static String bestFps = "\nBest FPS: ";
    static String worstFps = "\nWorst FPS: ";
    static String tris = "\nTriangle Count: ";

	mInfoHud->setCaption(currFps + StringConverter::toString(mpWindow->getLastFPS()) + 
		avgFps + StringConverter::toString(mpWindow->getAverageFPS()) +
		bestFps + StringConverter::toString(mpWindow->getBestFPS()) +
		worstFps + StringConverter::toString(mpWindow->getWorstFPS()) +
		tris + StringConverter::toString(mpWindow->getTriangleCount()));*/
	//mInfoHud->setCaption(currFps + StringConverter::toString(mpWindow->getLastFPS()));
}

MyGUI::OgrePlatform *TGameManager::getGUIPlatform()
{
	return mpPlatform;
}