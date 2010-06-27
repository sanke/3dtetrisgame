#ifndef _TBOARDBRICKI_H_
#define _TBOARDBRICKI_H_

#include "TBoardBrick.h"

const BRICK_MAP brickI[2] = { 
//   x|y
	{0,0,
	 1,0,
	 2,0,
	 3,0},
	{0,0,
	 0,1,
	 0,2,
	 0,3}
};

class TBoardBrickI :
	public TBoardBrick
{
public:
	TBoardBrickI(Ogre::SceneNode *brickSN);
	~TBoardBrickI(void);
protected:
	bool setBrickMap();
	bool setEntity();
	
	virtual void rotateCW();
};

#endif