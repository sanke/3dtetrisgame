#ifndef _TGAMEBOARD_H_
#define _TGAMEBOARD_H_

#include <Ogre.h>
#include <OIS.h>

#include "TBoardBrickFactory.h"

#define BOARD_SIZE 16

enum GameDifficulty {
		EASY = 16
};

enum QueryFlags {
		BRICK_FLAG = 1<<0,
		BOARD_FLAG = 1<<1,
		RENDER_RECT = 1<<2,
};

class TGameBoard : 
	public Ogre::Singleton<TGameBoard>, public Ogre::RenderTargetListener
{
public:
	TGameBoard(GameDifficulty diff);
	~TGameBoard(void);
public:
	void update(const Ogre::FrameEvent &evt);
	void load();
	void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	void mousePressed(OIS::MouseEvent const&e, OIS::MouseButtonID id);
	void mouseMove(const OIS::MouseEvent& e);

	void keyReleased(const OIS::KeyEvent& e);

	void initBoard();
	void destroyBoard();
	//void Put();
private:
	void startDragingBrick(int mouseX, int mouseY);
	void stopDragingBrick();
	void dragSelectedBrick(int mouseX, int mouseY);

	void updateBricks(const Ogre::FrameEvent &evt);
	
	bool placeBrick(TBoardBrick *brick,int bX,int bY);
	bool pickBrick(TBoardBrick *brick,int bX,int bY);
	
	Ogre::SceneNode	*clickToNode(int mouseX, int mouseY);
	TBoardBrick		*nodeToBrick(Ogre::SceneNode *sn);
	Ogre::String	InfoString;
	//void			InfoUpd(int mouseX, int mouseY);
	POINT ToTileCoords(TBoardBrick *brick, float x, float y);

	void setupBoardTiles();
private:
	Ogre::Root			*mpRoot;
	Ogre::SceneNode		*mpBoardNode;
	Ogre::SceneNode		*mpSelectedNode;
	Ogre::SceneManager	*mpSceneMng;
	Ogre::Camera		*mpCamera;
	Ogre::RaySceneQuery *mpRaySceneQuery;
	TBoardBrickFactory  *mpBrickFactory;
	bool				mbDragingBrick;

	Ogre::Vector2		mMove;
	Ogre::Ray			moveRay;
	Ogre::Vector3		pos;
	Ogre::TexturePtr	mpRenderTarget;

	Ogre::RenderTexture *mpRenderTexture;
	Ogre::Rectangle2D	*mpBgRect;
	Ogre::Entity		*mpBoardEntity;
	Ogre::SceneNode		*mpRenderNode;
	Ogre::MaterialPtr	mpBackground;
	// we should use boost library insted of std for cross platform compatability
	int					mpBoardMap[16][16];
protected:
	void preRenderTargetUpdate(const Ogre::RenderTargetEvent &evt);
	void postRenderTargetUpdate(const Ogre::RenderTargetEvent &evt);
};

#endif