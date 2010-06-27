#include "TGamePlayGlobals.h"
#include "TBoardBrickI.h"

using namespace Ogre;
//=====================================================================
TBoardBrickI::TBoardBrickI(Ogre::SceneNode *brickSN) : TBoardBrick(brickSN)
{
	setupBrick();
	mBrickID = BRICK_TYPE_I;
}
//=====================================================================
TBoardBrickI::~TBoardBrickI(void)
{
}
//=====================================================================
bool TBoardBrickI::setBrickMap()
{
	memcpy(&mBrickMap, &brickI,sizeof(BRICK_MAP));
	return true;
}
//=====================================================================
bool TBoardBrickI::setEntity()
{
	mpBrickEnt = Root::getSingletonPtr()->getSceneManager(PLAY_SCENE)->createEntity(BRICKI_ENT+StringConverter::toString(mBrickCount), BRICKI_MESH);
	if(!mpBrickEnt)
		return false;
	
	mpBrickSN->attachObject(mpBrickEnt);
	return true;
}
//=====================================================================
void TBoardBrickI::rotateCW()
{
	static unsigned short r=0;

	mpBrickSN->yaw(Radian(Degree(-90)));

	rotateDimensions();

	r++;
	if(r>((sizeof(brickI) / sizeof(BRICK_MAP)) - 1))
		r=0;

	memcpy(&mBrickMap, &brickI[r],sizeof(BRICK_MAP));
}
//=====================================================================
