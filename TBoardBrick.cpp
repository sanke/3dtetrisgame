#include "TBoardBrick.h"
#include "TGameBoard.h"
#include "TGamePlayGlobals.h"

using namespace Ogre;

unsigned int	TBoardBrick::mBrickCount = 0;
//int TBoardBrick::mTotalBrickNum = 0;


TBoardBrick::TBoardBrick(Ogre::SceneNode *brickSN)
{
	mpBrickSN = brickSN;
	mBrickCount++;
	mpBrickSN->setPosition(Vector3(0.0f,20.0f,0.0f));
	brickSN->setUserAny(Any(this));
	mbIsDying = false;
	mbNewBrick = true;
	//mpBrickEnt->setRenderQueueGroup(RENDER_QUEUE_6);
}
/**
* Initialize brick
*/
void TBoardBrick::setupBrick()
{
	setBrickMap();
	setEntity();
	mHalfWidth = getHalfWidth();
	mHalfHeight = getHalfHeight();
	mTileWidth = getWidthInTiles();
	mTileHeight = getHeightInTiles();
}
//=====================================================================
TBoardBrick::~TBoardBrick()
{
	
}
/**
* Destroy brick
*/
void TBoardBrick::destroyBrick()
{
	mbIsDying = true;
	mpBrickEnt->setMaterialName("FadeOut");

	mpSubEnt = mpBrickEnt->getSubEntity(0);
}
/**
* Update brick(should be called each frame)
*/
bool TBoardBrick::update(const FrameEvent &evt)
{
	if(die(evt))
		return false;

	return true;
}
/**
* Destroys brick
*/
bool TBoardBrick::die(const FrameEvent &evt)
{
	static float fade=1.0f;
	if(mbIsDying)
	{
		if(fade >= 0.0f) 
		{
			fade-=evt.timeSinceLastFrame;
			mpSubEnt->setCustomParameter(1, Vector4(fade,fade,fade,fade));
		}
		else 
		{
			fade = 1.0f;
			Root::getSingletonPtr()->getSceneManager(PLAY_SCENE)->destroySceneNode(mpBrickSN);
			return true;
		}
	}
	return false;
}
/**
* Gets bricks half width
*/
float TBoardBrick::getHalfWidth()
{
	AxisAlignedBox box = mpBrickEnt->getBoundingBox();
	return static_cast<int>(box.getHalfSize().x);
}
/**
* Gets bricks half height
*/
float TBoardBrick::getHalfHeight()
{
	AxisAlignedBox box = mpBrickEnt->getBoundingBox();
	return	static_cast<int>(box.getHalfSize().z);
}
/**
* Gets bricks witdh in tiles
*/
int TBoardBrick::getWidthInTiles()
{
	AxisAlignedBox box = mpBrickEnt->getBoundingBox();
	return static_cast<int>(box.getSize().x) / 10;
}
/**
* Gets bricks height in tiles
*/
int TBoardBrick::getHeightInTiles()
{
	AxisAlignedBox box = mpBrickEnt->getBoundingBox();
	return static_cast<int>(box.getSize().z) / 10;
}
/**
* Rotate bricks tile dimensions.
*/
void TBoardBrick::rotateDimensions()
{
	float tmp = mHalfWidth;
	mHalfWidth = mHalfHeight;
	mHalfHeight = tmp;

	int tmp2;
	tmp2 = mTileWidth;
	mTileWidth = mTileHeight;
	mTileHeight = tmp2;
}
//=====================================================================
SceneNode *TBoardBrick::getSceneNode()
{
	return mpBrickSN;
}
/**
* Check if brick is dying
*/
bool TBoardBrick::isDying()
{
	return mbIsDying;
}
/**
* Set bricks board position.
*/
void TBoardBrick::setBoardPos(POINT loc)
{
	Vector3 pos = mpBrickSN->getPosition();
	pos.x = loc.x * 10.0f + mHalfWidth-BOARD_HALFWIDTH;
	pos.z = loc.y * 10.0f + mHalfHeight-BOARD_HALFWIDTH;
	mpBrickSN->setPosition(pos);

	mTilePosition.x = loc.x;
	mTilePosition.y = loc.y;
}
//=====================================================================
Vector3 TBoardBrick::getBoardPos()
{
	return mpBrickSN->getPosition();
}
//=====================================================================
void TBoardBrick::setTilePos(POINT pos)
{
	
}
/**
* Set brick's altitude
*/
void TBoardBrick::setAltitude(float height)
{
	Vector3 pos = mpBrickSN->getPosition();
	pos.y = height;
	mpBrickSN->setPosition(pos);
}

POINT TBoardBrick::getTilePos()
{
	return mTilePosition;
}