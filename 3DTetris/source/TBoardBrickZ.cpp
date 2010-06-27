#include "TGamePlayGlobals.h"
#include "TBoardBrickZ.h"

using namespace Ogre;


//=====================================================================
TBoardBrickZ::TBoardBrickZ(Ogre::SceneNode *brickSN) : TBoardBrick(brickSN)
{
	setupBrick();
	mBrickID = BRICK_TYPE_Z;
}
//=====================================================================
TBoardBrickZ::~TBoardBrickZ(void)
{
	
}
//=====================================================================
bool TBoardBrickZ::setEntity()
{
	mpBrickEnt = Root::getSingletonPtr()->getSceneManager(PLAY_SCENE)->createEntity(BRICKZ_ENT+StringConverter::toString(mBrickCount), BRICKZ_MESH);
	if(!mpBrickEnt)
		return false;
	
	mpBrickSN->attachObject(mpBrickEnt);
	return true;
}
//=====================================================================
bool TBoardBrickZ::setBrickMap()
{
	memcpy(&mBrickMap, &brickZ,sizeof(BRICK_MAP));
	return true;
}
//=====================================================================
void TBoardBrickZ::rotateCW()
{
	static unsigned short r=0;

	mpBrickSN->yaw(Radian(Degree(-90)));

	rotateDimensions();

	r++;
	if(r>((sizeof(brickZ) / sizeof(BRICK_MAP)) - 1))
		r=0;

	memcpy(&mBrickMap, &brickZ[r],sizeof(BRICK_MAP));
}
//=====================================================================