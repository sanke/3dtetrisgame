#ifndef _TBOARDBRICK_H_
#define _TBOARDBRICK_H_

#include <Ogre.h>

//=====================================================
struct BRICK_POINT {
	int x,y;
};
//=====================================================
struct BRICK_MAP
{
	BRICK_POINT block_pos[4];
};
//=====================================================
// abstract brick class, all bricks should derive from this.
class TBoardBrick
{
public:
	TBoardBrick(Ogre::SceneNode *brickSN);
	~TBoardBrick(void);
public:
	BRICK_MAP				mBrickMap;
	BRICK_MAP				mBrickBoardMap;
	unsigned int			mBrickID;
	POINT					mBrickBoardPos;

	float					mHalfWidth,mHalfHeight;
	int						mTileWidth,mTileHeight;

	bool					mbNewBrick;
protected:
	Ogre::Entity			*mpBrickEnt;
	Ogre::SceneNode			*mpBrickSN;
	Ogre::SubEntity			*mpSubEnt;

	bool					mbIsDying;
	static unsigned int		mBrickCount;	
	POINT					mTilePosition;
protected:
	virtual bool	setBrickMap() = 0;
	virtual bool	setEntity() = 0;

	virtual void	rotateDimensions();
	virtual	void	setupBrick();
	
	bool			die(const Ogre::FrameEvent &evt);
public:
	virtual void	rotateCW() = 0;
	virtual bool	update(const Ogre::FrameEvent &evt);
	virtual void	destroyBrick();
	
	virtual float	getHalfWidth();
	virtual float	getHalfHeight();
	virtual int		getWidthInTiles();
	virtual int		getHeightInTiles();

	Ogre::SceneNode *getSceneNode();

	Ogre::Vector3	getBoardPos();
	void			setBoardPos(POINT loc);
	POINT			getTilePos();
	void			setTilePos(POINT pos);
	void			setAltitude(float height);
	bool			isDying();
};

#endif