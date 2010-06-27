#ifndef _TBOARDBRICKFACTORY_H_
#define _TBOARDBRICKFACTORY_H_

#include "TBoardBrickZ.h"
#include "TBoardBrickI.h"
#include "TGamePlayGlobals.h"
#include <Ogre.h>

//brick factory

class TBoardBrickFactory : public Ogre::Singleton<TBoardBrickFactory>
{
public:
	TBoardBrickFactory(Ogre::SceneManager *sceneManager, Ogre::SceneNode *boardScene);
	~TBoardBrickFactory(void);
public:
	TBoardBrick *craftBrick(BRICK_TYPE type);
	void		destroyBrick(TBoardBrick *brick);
	void		destroyFactory();
	void		updateBricks(const Ogre::FrameEvent &evt);
private:
	Ogre::SceneManager	*mpSceneMng;
	Ogre::SceneNode		*mpBoardScene;

	std::list<TBoardBrick*>		mBrickList;
	std::list<TBoardBrick*>		mDeleteList;
	unsigned int mBrickCount;
private:
	void setupBrick(TBoardBrick *brick);
};

#endif