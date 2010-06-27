#ifndef _TGAMESTATE_H_
#define _TGAMESTATE_H_

#include <Ogre.h>
#include <OIS.h>

#include "TGameManager.h"

// abstract class for game states.

class TGameState
{
protected:
	TGameState() {};
protected:
	Ogre::Root		*mpRoot;
	//Ogre::RenderWindow *mpWindow;
public:
	virtual void enter() = 0;
	virtual void exit() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual bool keyPressed(const OIS::KeyEvent& e) = 0;
	virtual bool keyReleased(const OIS::KeyEvent& e) = 0;

	virtual bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id) = 0;
	virtual bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id) = 0;
	virtual bool mouseMoved(const OIS::MouseEvent& e) = 0;

	virtual bool frameStarted(const Ogre::FrameEvent &evt) = 0;
	virtual bool frameEnded(const Ogre::FrameEvent &evt) = 0;

	void changeGameState(TGameState *state) 
	{
		TGameManager::getSingletonPtr()->changeGameState(state);
	}

	void pushGameState(TGameState *state)
	{
		TGameManager::getSingletonPtr()->pushGameState(state);
	}

	void popGameState()
	{
		TGameManager::getSingletonPtr()->popGameState();
	}

};

#endif