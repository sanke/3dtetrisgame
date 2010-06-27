#include "TGameMenu.h"
#include "TGameManager.h"
#include "TGamePlay.h"


using namespace Ogre;

SceneManager		*TGameMenu::mpSceneMng;
Camera				*TGameMenu::mpCamera;
Viewport			*TGameMenu::mpViewport;
MyGUI::Gui			*TGameMenu::mpGUI;
MyGUI::WindowPtr	TGameMenu::mMenuWindow;

void TGameMenu::setupCamera()
{
	mpCamera = mpSceneMng->createCamera("PlayCamera");
	mpCamera->setPosition(Vector3(0.0f,0.0f,0.0f));
	mpCamera->lookAt(Vector3(0.0f,0.0f,0.0f));

	mpCamera->setNearClipDistance(5.0f);
	mpCamera->setFarClipDistance(1000.0f);
}


void TGameMenu::setupSceneMng()
{
	mpSceneMng = mpRoot->getSceneManager("PlayScene");
}

void TGameMenu::setupViewport()
{
	mpViewport = mpRoot->getAutoCreatedWindow()->getViewport(0);
	mpViewport->setCamera(mpCamera);
	//mpViewport->setClearEveryFrame(true);
	//mpViewport = mpRoot->getAutoCreatedWindow()->getViewport(1);
	mpViewport->setBackgroundColour(ColourValue(0.0f,0.0f,0.0f));

	mpCamera->setAspectRatio(Real(mpViewport->getActualWidth()) / Real(mpViewport->getActualHeight()));
}

void TGameMenu::preSetupGUI()
{
	mpGUI = MyGUI::Gui::getInstancePtr();
	
	
	//mpGUI->setSceneManager(mpSceneMng);
	mMenuWindow = mpGUI->createWidget<MyGUI::Window>("FlowConteiner", 300,150, 300, 300, MyGUI::Align::Center, "Main", "Menu");
}