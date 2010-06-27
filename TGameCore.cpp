#include "TGameCore.h"

#include "TGameBoard.h"

template<> TGameCore* Singleton<TGameCore>::ms_Singleton = 0;

//=============================================
// loads config.cfg, setups resource paths
bool TGameCore::LoadConfig(String configFile)
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
	//Render
	return true;
}
//=============================================
// Need to uncoment if our core goes into DLL
//TGameCore *TGameCore::getSingletonPtr()
//{
//	return ms_Singleton;
//}
//
//TGameCore &TGameCore::getSingleton()
//{
//	assert(ms_Singleton);
//	return *ms_Singleton;
//}
//=============================================
// Initialize orge engine ant other stuff like gui, input
bool TGameCore::Initialize()
{
	ParamList OISParams;

	mpRoot = new Root;

	LoadConfig("config.cfg");

	if(!mpRoot->showConfigDialog())
	{
		delete mpRoot;
		return false;
	}

	mpMainWnd = mpRoot->initialise(true, WINDOW_TITLE);
	
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	
	CreateMainScene();

	mpMainWnd->getCustomAttribute("WINDOW", &mWndHandle);

	std::ostringstream strWnd;

	SetupCamera();
	SetupBoard();
	SetupGUI();

	strWnd << mWndHandle;

	OISParams.insert(std::make_pair(std::string("WINDOW"), strWnd.str()));

	mpInputManager = OIS::InputManager::createInputSystem(OISParams);

	mpKeyboard = static_cast<OIS::Keyboard*>(mpInputManager->createInputObject(OISKeyboard, true));
	mpMouse = static_cast<OIS::Mouse*>(mpInputManager->createInputObject(OISMouse,true));

	mpMainWnd->getMetrics(mWidth, mHeight, mDepth, mLeft, mTop);
	
	mpKeyboard->setEventCallback(this);

	mpMouse->setEventCallback(this);


	mpRoot->addFrameListener(this);

	return true;
}
//=============================================
// Create main scene of game 
void TGameCore::CreateMainScene()
{
	mpMainSceneMng = mpRoot->createSceneManager(ST_GENERIC, "Main");
	_Log->logMessage("Main scene created");
	//mpMainSceneMng->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
	//mpMainSceneMng->setAmbientLight(ColourValue(1.0f,1.0f,1.0f));
}
//=============================================
// Setup camera and viewport
bool TGameCore::SetupCamera()
{
	mpCamera = mpMainSceneMng->createCamera("Main");
	mpCamera->setPosition(Vector3(-300.0f,300.0f,300.0f));
	mpCamera->lookAt(Vector3(0.0f,0.0f,0.0f));
	mpCamera->setNearClipDistance(5.0f);
	mpCamera->setFarClipDistance(5000.0f);
	
	mpViewport = mpMainWnd->addViewport(mpCamera);
	
	mpViewport->setBackgroundColour(ColourValue(0.0f,0.0f,0.0f));
	mpCamera->setAspectRatio(Real(mpViewport->getActualWidth()) / Real(mpViewport->getActualHeight()));

	return true;
}
//=============================================
// Setup CEGUI and set mouse cursor
bool TGameCore::SetupGUI()
{
	mpGUIRenderer = new CEGUI::OgreCEGUIRenderer(mpMainWnd, RENDER_QUEUE_OVERLAY, false, 3000, mpMainSceneMng);
	mpGUISystem = new CEGUI::System(mpGUIRenderer);
	
	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme",(CEGUI::utf8*)"General" );
	
	mpGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
	mpGUISystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-10");

	CEGUI::MouseCursor::getSingleton().setImage(CEGUI::System::getSingleton().getDefaultMouseCursor());

	//temp(for stat window)
	CEGUI::WindowManager *win = CEGUI::WindowManager::getSingletonPtr();   
	CEGUI::Window *fpsW = win->createWindow("DefaultGUISheet", "CEGUIDemo/Sheet");

	FPS = win->createWindow("TaharezLook/StaticText", "CEGUIDemo/QuitButton");
	FPS->setText("FPS:");
	FPS->setSize(CEGUI::UVector2(CEGUI::UDim(0.18, 0), CEGUI::UDim(0.15, 0)));

	fpsW->addChildWindow(FPS);
	mpGUISystem->setGUISheet(fpsW);

	return true;
}
//=============================================
// Start our listener loop(game loop)
bool TGameCore::Start()
{
	mpRoot->startRendering();

	Shutdown();

	return true;
}
//=============================================
bool TGameCore::SetupBoard()
{
	mpMainSceneMng->createSceneNode("Board");
	new TGameBoard();
	TGameBoard::getSingletonPtr()->Load();
	return true;
}
//=============================================
bool TGameCore::frameStarted(const FrameEvent &evt)
{
	mpKeyboard->capture();
	mpMouse->capture();
	
	TGameBoard::getSingletonPtr()->Update();
	
	// temp stuff basic camera manipulation for testing...
	// start
	Vector3 delta(1.0f,0.0f,0.0f);
	OIS::MouseState state = mpMouse->getMouseState();
	UpdateStatWnd();
	Vector3 dir = mpCamera->getDirection();
	dir = dir.normalisedCopy() / 5.0f;
	Vector3 m = Vector3::UNIT_Y;
	if(state.buttonDown(MB_Right))
	{
		mpCamera->yaw(Degree(state.X.rel*-(evt.timeSinceLastFrame*camSpeed/50)));
		mpCamera->pitch(Degree(state.Y.rel*-(evt.timeSinceLastFrame*camSpeed/50)));
	}
	if(mpKeyboard->isKeyDown(KC_A))
	{
		Vector3 strafe = mpCamera->getUp().crossProduct(mpCamera->getDirection());
		
		mpCamera->move(strafe.normalisedCopy()*evt.timeSinceLastFrame*camSpeed);
	}
	if(mpKeyboard->isKeyDown(KC_D))
	{
		Vector3 strafe = mpCamera->getUp().crossProduct(mpCamera->getDirection());
		//Quaternion q(
		//mpCamera->rotate(
		mpCamera->move(-strafe.normalisedCopy()*evt.timeSinceLastFrame*camSpeed);
		
	}
	if(mpKeyboard->isKeyDown(KC_W))
	{
		mpCamera->move(dir*evt.timeSinceLastFrame*camSpeed);
	}
	if(mpKeyboard->isKeyDown(KC_S))
	{
		mpCamera->move(-dir*evt.timeSinceLastFrame*camSpeed);
	}
	if(mpKeyboard->isKeyDown(KC_ESCAPE))
	{
		return false;
	}
	// end

	return true;
}

void TGameCore::UpdateStatWnd()
{
	static String currFps = "Current FPS: ";
    static String avgFps = "\nAverage FPS: ";
    static String bestFps = "\nBest FPS: ";
    static String worstFps = "\nWorst FPS: ";
    static String tris = "\nTriangle Count: ";

	FPS->setText(currFps + StringConverter::toString(mpMainWnd->getLastFPS()) + 
		avgFps + StringConverter::toString(mpMainWnd->getAverageFPS()) +
		bestFps + StringConverter::toString(mpMainWnd->getBestFPS()) +
		worstFps + StringConverter::toString(mpMainWnd->getWorstFPS()) +
		tris + StringConverter::toString(mpMainWnd->getTriangleCount()));

	//FPS->setText(currFps + StringConverter::toString(mpMainWnd->getLastFPS()));
}
//=============================================
bool TGameCore::frameRenderingQueued(const FrameEvent &evt)
{
	return true;
}
//=============================================
bool TGameCore::frameEnded(const FrameEvent &evt)
{
	return true;
}
//=============================================
bool TGameCore::keyPressed(const KeyEvent &e)
{
	CEGUI::System::getSingletonPtr()->injectKeyDown(e.key);
	CEGUI::System::getSingletonPtr()->injectChar(e.text);

	return true;
}
//=============================================
bool TGameCore::keyReleased(const KeyEvent &e)
{
	//temp stuff
	if(e.key == KC_M)
	{
		if(mpCamera->getPolygonMode()!=PM_WIREFRAME)
			mpCamera->setPolygonMode(PM_WIREFRAME);
		else
			mpCamera->setPolygonMode(PM_SOLID);
	}
	//end
	CEGUI::System::getSingletonPtr()->injectKeyUp(e.key);

	return true;
}
//=============================================
bool TGameCore::mouseMoved(const MouseEvent& e)
{

	CEGUI::System::getSingleton().injectMouseMove(e.state.X.rel, e.state.Y.rel);
	
	return true;
}
//=============================================
bool TGameCore::mousePressed(const MouseEvent& e, OIS::MouseButtonID id)
{
	RaySceneQuery *mpRaySceneQuery = mpSceneMng->createRayQuery(Ray());
	Ray mouseRay  = mpCamera->getCameraToViewportRay(e.state.X.abs / e.state.width,e.state.Y.abs / e.state.height);
	
	//mouseRay.setDirection(mpCamera->getDirection());
	mpRaySceneQuery->setRay(mouseRay);
	//mpRaySceneQuery->setSortByDistance(true);
	
	RaySceneQueryResult result = mpRaySceneQuery->execute();
	
	RaySceneQueryResult::iterator iter = result.begin();
	
	for(iter=result.begin();iter != result.end(); iter++)
	{
		String name = iter->movable->getName();
		
	}
	CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));

	return true;
}
//=============================================
bool TGameCore::mouseReleased(const MouseEvent& e, OIS::MouseButtonID id)
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));

	return true;
}
//=============================================
void TGameCore::Shutdown()
{
	mpInputManager->destroyInputObject(mpKeyboard);
	mpKeyboard = 0;
	mpInputManager->destroyInputObject(mpMouse);
	mpMouse = 0;
	InputManager::destroyInputSystem(mpInputManager);
	mpInputManager = 0;
	
	delete mpRoot;
	delete this;
}
//=============================================
CEGUI::MouseButton TGameCore::convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
    case OIS::MB_Right:
        return CEGUI::RightButton;
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
    default:
        return CEGUI::LeftButton;
    }
}
