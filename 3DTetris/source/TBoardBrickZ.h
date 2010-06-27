#ifndef _TBOARDBRICKZ_H_
#define _TBOARDBRICKZ_H_

#include "TBoardBrick.h"

const BRICK_MAP brickZ[2] = { 
//   x|y
	{0,0,
	 1,0,
	 1,1,
	 2,1},
	{1,0,
	 1,1,
	 0,1,
	 0,2}
};

class TBoardBrickZ :
	public TBoardBrick
{       
public:
	TBoardBrickZ(Ogre::SceneNode *brickSN);
	~TBoardBrickZ(void);
protected:
	bool setBrickMap();
	bool setEntity();

	virtual void rotateCW();
};

#endif