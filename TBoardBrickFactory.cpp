#include "TBoardBrickFactory.h"


TBoardBrickFactory * Ogre::Singleton<class TBoardBrickFactory>::ms_Singleton = 0;

using namespace Ogre;
using namespace std;

//=====================================================================
TBoardBrickFactory::TBoardBrickFactory(Ogre::SceneManager *sceneManager, Ogre::SceneNode *boardScene)
{
	mpSceneMng = sceneManager;
	mpBoardScene = boardScene;
	mBrickCount=0;
}
//=====================================================================
TBoardBrickFactory::~TBoardBrickFactory(void)
{
}
//=====================================================================
TBoardBrick *TBoardBrickFactory::craftBrick(BRICK_TYPE type)
{
	TBoardBrick *newBrick=0;
	SceneNode *brickSN;

	brickSN = mpBoardScene->createChildSceneNode();
	mBrickCount++;

	switch(type)
	{
	case BRICK_TYPE_Z:
		newBrick = new TBoardBrickZ(brickSN);
		setupBrick(newBrick);
		return newBrick;
		break;
	case BRICK_TYPE_I:
		newBrick = new TBoardBrickI(brickSN);
		setupBrick(newBrick);
		return newBrick;
		break;
	// and others...
	}
	mBrickCount--;
	return 0;
}
//=====================================================================
void TBoardBrickFactory::destroyBrick(TBoardBrick *brick)
{
	brick->destroyBrick();
}
//=====================================================================
void TBoardBrickFactory::setupBrick(TBoardBrick *brick)
{
	mBrickList.push_back(brick);
}
//=====================================================================
void TBoardBrickFactory::destroyFactory()
{
	std::list<TBoardBrick*>::iterator iter;
	for(iter=mBrickList.begin();iter!=mBrickList.end();iter++)
		delete (*iter);
}
//=====================================================================
void TBoardBrickFactory::updateBricks(const FrameEvent &evt)
{
	std::list<TBoardBrick*>::iterator iter;
	
	for(iter=mBrickList.begin();iter!=mBrickList.end();iter++)
	{
		if(!(*iter)->update(evt))
		{
			mDeleteList.push_back((*iter));
		}
	}
	TBoardBrick *brick;
	while(!mDeleteList.empty())
	{
		brick = mDeleteList.back();
		mBrickList.remove(brick);
		delete brick;
		mDeleteList.pop_back();
	}
}
//=====================================================================