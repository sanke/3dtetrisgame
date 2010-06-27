#include "TGamePlay.h"
#include "TGameBoard.h"
#include "TGameManager.h"
#include "TGamePlayGlobals.h"

using namespace Ogre;

TGamePlay TGamePlay::mGamePlay;

//=====================================================================
TGamePlay::TGamePlay(void)
{
}
//=====================================================================
TGamePlay::~TGamePlay(void)
{
}
//=====================================================================
void TGamePlay::enter()
{
	mbShutdown = false;

	mpRoot = Root::getSingletonPtr();

	setupSceneMng();
	setupCamera();
	setupViewport();

	new TGameBoard(EASY);
	

	setupGUI();
}
//=====================================================================
void TGamePlay::exit()
{
	TGameBoard::getSingletonPtr()->destroyBoard();

	mpSceneMng->clearScene();
	mpRoot->destroySceneManager(mpSceneMng);
}
//=====================================================================
void TGamePlay::pause()
{

}
//=====================================================================
void TGamePlay::resume()
{

}
//=====================================================================
bool TGamePlay::frameStarted(const FrameEvent &evt)
{
	//mpGUI->injectFrameEntered(evt.timeSinceLastFrame);
	if(mbShutdown)
		return false;
	
	TGameBoard::getSingletonPtr()->update(evt);

	return true;
}
//=====================================================================
void TGamePlay::updateCamera(const OIS::MouseEvent &e)
{
	if(e.state.buttonDown(OIS::MB_Right))
	{
		mpGUI->setVisiblePointer(false);
		mAngle = Radian(Degree(e.state.X.rel *0.6));
		Quaternion rY(mAngle, Vector3::UNIT_Y);
		//rY.getYaw(false);
	/*	mAngle = Radian(Degree(e.state.Y.rel *0.6));
		Quaternion rX(mAngle, Vector3::UNIT_X);
		rY = rY * rX;*/
		mpCameraParent->rotate(rY);
	}
}
//=====================================================================
bool TGamePlay::frameEnded(const FrameEvent &evt)
{

	return true;
}
//=====================================================================
bool TGamePlay::keyPressed(const OIS::KeyEvent &e)
{
	//mpGUI->injectKeyPress(e);
	return true;
}
//=====================================================================
bool TGamePlay::keyReleased(const OIS::KeyEvent &e)
{
	//mpGUI->injectKeyRelease(e);

	switch(e.key)
	{
	case OIS::KC_ESCAPE:
		mbShutdown = true;
		break;
	case OIS::KC_M:
		if(mpCamera->getPolygonMode() == PM_WIREFRAME)
			mpCamera->setPolygonMode(PM_POINTS);
		else if(mpCamera->getPolygonMode() == PM_SOLID)
			mpCamera->setPolygonMode(PM_WIREFRAME);
		else if(mpCamera->getPolygonMode() == PM_POINTS)
			mpCamera->setPolygonMode(PM_SOLID);
	}

	TGameBoard::getSingletonPtr()->keyReleased(e);

	return true;
}
//=====================================================================
bool TGamePlay::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	TGameBoard::getSingletonPtr()->mousePressed(e,id);
	//mpGUI->injectMousePress(e,id);
	return true;
}
//=====================================================================
bool TGamePlay::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	if(!mpGUI->isVisiblePointer() && id == OIS::MB_Right)
	{
		mpGUI->setVisiblePointer(true);
	}
	TGameBoard::getSingletonPtr()->mouseReleased(e,id);
	//mpGUI->injectMouseRelease(e,id);
	return true;
}
//=====================================================================
bool TGamePlay::mouseMoved(const OIS::MouseEvent &e)
{
	updateCamera(e);
	TGameBoard::getSingletonPtr()->mouseMove(e);
	
	//mpGUI->injectMouseMove(e);
	return true;
}
//=====================================================================
void TGamePlay::setupSceneMng()
{
	mpSceneMng = mpRoot->createSceneManager(ST_GENERIC, PLAY_SCENE);
	TGameManager::getSingletonPtr()->getGUIPlatform()->getRenderManagerPtr()->setSceneManager(mpSceneMng);
	mpSceneMng->setAmbientLight(ColourValue(1.0f,1.0f,1.0f));
}
//=====================================================================
void TGamePlay::setupCamera()
{
	mpCamera = mpSceneMng->createCamera(PLAY_CAMERA);
	
	//mpCamera->setPosition(Vector3(300.0f,0.0f,0.0f));
	mpCamera->lookAt(Vector3(0.0f,0.0f,0.0f));
	mpCamera->setOrientation(Quaternion::IDENTITY);
	
	mpCamera->setNearClipDistance(5.0f);
	mpCamera->setFarClipDistance(1000.0f);
	mpCamera->setQueryFlags(1<<3);
	mpCameraParent = mpSceneMng->getRootSceneNode()->createChildSceneNode();
	mpCameraNode = mpCameraParent->createChildSceneNode();
	mpCameraNode->attachObject(mpCamera);
	mpCameraNode->translate(Vector3(0.0f,300.0f,300.0f));
	mpCameraNode->lookAt(Vector3(0,0,0), Node::TS_WORLD);
}
//=====================================================================
void TGamePlay::setupViewport()
{
	//TODO..
	
	mpViewport = TGameManager::getSingletonPtr()->mpViewport;
	mpViewport->setCamera(mpCamera);
	//mpViewport->setClearEveryFrame(true);
	mpViewport->setBackgroundColour(ColourValue(0.0f,0.0f,0.0f));

	mpCamera->setAspectRatio(Real(mpViewport->getActualWidth()) / Real(mpViewport->getActualHeight()));
}
//=====================================================================
void TGamePlay::setupGUI()
{
	mpGUI = MyGUI::Gui::getInstancePtr();
	
	//mpGUI->setSceneManager(mpSceneMng);
}
//=====================================================================
