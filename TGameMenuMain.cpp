#include "TGameMenuMain.h"
#include "TGameManager.h"
#include "TGamePlay.h"

using namespace Ogre;

TGameMenuMain TGameMenuMain::mGameMenuMain;


TGameMenuMain::TGameMenuMain(void)
{
}

TGameMenuMain::~TGameMenuMain(void)
{
}

void TGameMenuMain::enter()
{
	mpRoot = Root::getSingletonPtr();
	mpGUI = MyGUI::Gui::getInstancePtr();

	setupSceneMng();
	setupCamera();
	setupViewport();
	setupGUI();
}

void TGameMenuMain::exit()
{
	mpGUI->destroyChildWidget(mMenuWindow);
	mpSceneMng->clearScene();
	mpRoot->destroySceneManager(mpSceneMng);
}

void TGameMenuMain::pause()
{
	
}

void TGameMenuMain::resume()
{

}
/**
* Setup game menu GUI.
*/
void TGameMenuMain::setupGUI()
{
	preSetupGUI();
	MyGUI::ButtonPtr buttonStart = mMenuWindow->createWidget<MyGUI::Button>("Button", 5, 5, 50, 25, MyGUI::Align::Center, "Start");
	buttonStart->setCaption("Play");
	buttonStart->eventMouseButtonClick = MyGUI::newDelegate(this, &TGameMenuMain::buttonStartClick);
	MyGUI::ButtonPtr buttonQuit = mMenuWindow->createWidget<MyGUI::Button>("Button", 5, 30, 50, 25, MyGUI::Align::Center, "Quit");
	buttonQuit->setCaption("Exit");
	buttonQuit->eventMouseButtonClick = MyGUI::newDelegate(this, &TGameMenuMain::buttonQuitClick);
}

void TGameMenuMain::buttonStartClick(MyGUI::WidgetPtr _widget)
{
	TGameManager::getSingletonPtr()->changeGameState(TGamePlay::getInst());
	//TGameManager::getSingletonPtr()->pushGameState(TGamePlay::getInst());
}

void TGameMenuMain::buttonQuitClick(MyGUI::WidgetPtr _widget)
{
	//TGameManager::getSingletonPtr()->clearStates();
	mbShutdown = true;
}