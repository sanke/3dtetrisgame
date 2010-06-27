#include "TGameBoard.h"
#include "TGameManager.h"
#include "TGamePlayGlobals.h"
#include "TGameManager.h"

using namespace Ogre;

template<> TGameBoard* Singleton<TGameBoard>::ms_Singleton = 0;

//=====================================================================
TGameBoard::TGameBoard(GameDifficulty diff)
{	
	mpRoot = Root::getSingletonPtr();
	
	mpSceneMng = mpRoot->getSceneManager(PLAY_SCENE);
	mpCamera = mpSceneMng->getCamera(PLAY_CAMERA);
	
	mbDragingBrick = false;
	mpSelectedNode = 0;
	
	initBoard();
}
//=====================================================================
TGameBoard::~TGameBoard(void)
{
	
}

//=====================================================================
void TGameBoard::load()
{
	// ?
}
//=====================================================================
void TGameBoard::update(const FrameEvent &evt)
{
	

	updateBricks(evt);
}	
//=====================================================================
void TGameBoard::mousePressed(OIS::MouseEvent const&e, OIS::MouseButtonID id)
{
	if(id == OIS::MB_Left)
	{
		startDragingBrick(e.state.X.abs, e.state.Y.abs);
	}
}
//=====================================================================
void TGameBoard::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	stopDragingBrick();
}
//=====================================================================
void TGameBoard::mouseMove(const OIS::MouseEvent &e)
{
	dragSelectedBrick(e.state.X.abs,e.state.Y.abs);

}
//=====================================================================
void TGameBoard::destroyBoard()
{
	//for(int i=0;i<BOARD_SIZE;i++)
	//	delete [] mpBoardMap[i];

	//delete [] mpBoardMap;

	mpBrickFactory->destroyFactory();
	delete mpBrickFactory;
}
//=====================================================================
void TGameBoard::initBoard()
{
	mpBoardEntity = mpSceneMng->createEntity(PLAY_BOARD_ENTITY, PLAY_BOARD_MESH);
	mpBoardEntity->setQueryFlags(BOARD_FLAG);
	//mpBoardEntity->setRenderQueueGroup(RENDER_QUEUE_MAX);
	mpBoardNode = mpSceneMng->getRootSceneNode()->createChildSceneNode(PLAY_BOARD_NODE);
	mpBoardNode->attachObject(mpBoardEntity);
	
	mpBrickFactory = new TBoardBrickFactory(mpSceneMng, mpBoardNode);
	
	//mpRenderTarget = TextureManager::getSingletonPtr()->createManual("RenderTarget",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	//	TEX_TYPE_2D, TGameManager::getSingletonPtr()->mWidth, TGameManager::getSingletonPtr()->mHeight,
	//	0,PF_FLOAT16_RGBA, TU_RENDERTARGET);
	//
	//mpRenderTexture = mpRenderTarget->getBuffer()->getRenderTarget();
	//mpRenderTexture->addViewport(mpCamera);
	//mpRenderTexture->getViewport(0)->setClearEveryFrame(true);
	//mpRenderTexture->getViewport(0)->setBackgroundColour(ColourValue::Black);
	//mpRenderTexture->getViewport(0)->setOverlaysEnabled(false);
	//mpRenderTexture->setAutoUpdated(true);
	
	mpBgRect = new Rectangle2D(true);

	mpBgRect->setCorners(-1.0f, 1.0f, 1.0f, -1.0f);

	mpBgRect->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE));
	
	mpBgRect->setQueryFlags(RENDER_RECT);

	mpRenderNode = mpSceneMng->getRootSceneNode()->createChildSceneNode("Bg");
	
	mpRenderNode->attachObject(mpBgRect);

	mpBackground = MaterialManager::getSingletonPtr()->create("BgMat",  ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	mpBackground->setDepthWriteEnabled(false);
	Ogre::Technique *tech = mpBackground->createTechnique();

	tech->createPass();
	mpBackground->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	mpBackground->getTechnique(0)->getPass(0)->createTextureUnitState("Bg_1.png");
	
	
	mpBgRect->setMaterial("BgMat");
	mpBgRect->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);
//	mpRenderTexture->addListener(this);
	
//	mpBgRect->setVisible(false);
	setupBoardTiles();
}
//=====================================================================
void TGameBoard::preRenderTargetUpdate(const RenderTargetEvent &evt)
{
	//mpBoardEntity->setVisible(true);
	//mpBoardNode->setVisible(true);
}
//=====================================================================
void TGameBoard::postRenderTargetUpdate(const RenderTargetEvent &evt)
{
	//mpBoardEntity->setVisible(false);
	//mpBoardNode->setVisible(false);
}
//=====================================================================
void TGameBoard::startDragingBrick(int mouseX, int mouseY)
{
	if(mpSelectedNode || mbDragingBrick)
		return;

	mpSelectedNode = clickToNode(mouseX, mouseY);

	if(!mpSelectedNode)
		return;
	
	TBoardBrick *brick = nodeToBrick(mpSelectedNode);
	
	if(brick->isDying())
	{
		mpSelectedNode = 0;
		return;
	}

	POINT pos = brick->getTilePos();
	pickBrick(brick, pos.x, pos.y);

	mbDragingBrick = true;

	brick->setAltitude(25.0f);
}
//=====================================================================
void TGameBoard::stopDragingBrick()
{
	if(!mbDragingBrick || !mpSelectedNode)
		return;

	TBoardBrick *brick = nodeToBrick(mpSelectedNode);
	
	POINT pos = brick->getTilePos();
	
	if(placeBrick(brick, pos.x, pos.y))
		brick->setAltitude(15.0f);
	else
		brick->destroyBrick();
	
	InfoString = StringConverter::toString(pos.x) + " " +  StringConverter::toString(pos.y);
	//TGameManager::getSingletonPtr()->mInfoHud->setCaption(InfoString);
	mbDragingBrick = false;
	mpSelectedNode = 0;
}
//=====================================================================
void TGameBoard::dragSelectedBrick(int mouseX, int mouseY)
{
	if(!mbDragingBrick || !mpSelectedNode)
		return;

	Ray mouseRay  = mpCamera->getCameraToViewportRay( Real((static_cast<float>(mouseX) / TGameManager::getSingletonPtr()->mWidth)), Real(static_cast<float>(mouseY) / TGameManager::getSingletonPtr()->mHeight));
	Vector3 position = mouseRay.getOrigin() + mouseRay.getDirection() * ((-mouseRay.getOrigin().y) / (mouseRay.getDirection().y));
	
	TBoardBrick *brick = nodeToBrick(mpSelectedNode);
	
	POINT pos = ToTileCoords(brick, position.x, position.z);

	brick->setBoardPos(pos);
}
//=====================================================================
void TGameBoard::keyReleased(const OIS::KeyEvent &e)
{
	if(e.key == OIS::KC_SPACE)
	{
		TBoardBrick *brick = mpBrickFactory->craftBrick(BRICK_TYPE_Z);
	}
	else if(e.key == OIS::KC_B)
	{
		TBoardBrick *brick = mpBrickFactory->craftBrick(BRICK_TYPE_I);
	}

	if(mbDragingBrick && e.key == OIS::KC_R)
	{
		any_cast<TBoardBrick*>(mpSelectedNode->getUserAny())->rotateCW();
		
	}
}
//=====================================================================
// Places brick to a tile map of board. Here we will check if brick is placeable.
// Each cell of boardmap holds reference to a brick.
bool TGameBoard::placeBrick(TBoardBrick *brick, int bX, int bY)
{
	int x,y;
	int i;

	if(bX < 0 || bX > BOARD_SIZE-brick->mTileWidth || bY < 0 || bY > BOARD_SIZE-brick->mTileHeight)
		return false;

	for(i=0;i<4;i++)
	{
  		x = brick->mBrickMap.block_pos[i].x+bX;
		y = brick->mBrickMap.block_pos[i].y+bY;
		
		if(mpBoardMap[y][x])
			return false;
	}

	brick->mbNewBrick = false;
	
	for(i=0;i<4;i++)
	{
 		x = brick->mBrickMap.block_pos[i].x+bX;
		y = brick->mBrickMap.block_pos[i].y+bY;
		
		brick->mBrickBoardMap.block_pos[i].x = x;
		brick->mBrickBoardMap.block_pos[i].y = y;

		mpBoardMap[y][x] = brick->mBrickID;
	}
	return true;
}
//=====================================================================
bool TGameBoard::pickBrick(TBoardBrick *brick, int bX,int bY)
{	
	int x,y;
	
	if(brick->mbNewBrick)
		return true;

	for(int i=0;i<4;i++)
	{
		x = brick->mBrickBoardMap.block_pos[i].x;
		y = brick->mBrickBoardMap.block_pos[i].y;
	
		mpBoardMap[y][x]=0;
	}
	return true;
}
//=====================================================================
void TGameBoard::setupBoardTiles()
{
	int i;
	//mpBoardMap = new int*[BOARD_SIZE+1];
	//
	//
	//for(i=0;i<BOARD_SIZE+1;i++)
	//	mpBoardMap[i]=new int[BOARD_SIZE+1];
	for(int j=0;j<BOARD_SIZE;j++)
	{
		for(i=0;i<BOARD_SIZE;i++)
			mpBoardMap[j][i]=0;
	}
}
//=====================================================================
void TGameBoard::updateBricks(const FrameEvent &evt)
{
	mpBrickFactory->updateBricks(evt);
}
//=====================================================================
POINT TGameBoard::ToTileCoords(TBoardBrick *brick, float x, float y)
{
	POINT pos;
	pos.x = Math::Floor((x+BOARD_HALFWIDTH-brick->mHalfWidth) / 10.0f + 0.5f);
	pos.y = Math::Floor((y+BOARD_HALFWIDTH-brick->mHalfHeight) / 10.0f+ 0.5f);

	return pos;
}
//=====================================================================
SceneNode *TGameBoard::clickToNode(int mouseX, int mouseY)
{
	Ray mouseRay  = mpCamera->getCameraToViewportRay( Real((static_cast<float>(mouseX) / TGameManager::getSingletonPtr()->mWidth)), Real(static_cast<float>(mouseY) / TGameManager::getSingletonPtr()->mHeight));
	
	RaySceneQuery *mpRaySceneQuery = mpSceneMng->createRayQuery(mouseRay);
	
	mpRaySceneQuery->setQueryMask(BRICK_FLAG);
	mpRaySceneQuery->setSortByDistance(true);

	RaySceneQueryResult result = mpRaySceneQuery->execute();

	if(result.size())
	{
		RaySceneQueryResult::iterator iter = result.begin();

		if(iter->movable)
		{	
			return iter->movable->getParentSceneNode();
		}
	}
	return 0;
}
//=====================================================================
TBoardBrick *TGameBoard::nodeToBrick(Ogre::SceneNode *sn)
{
	return any_cast<TBoardBrick*>(sn->getUserAny());
}
