#ifndef _TGAMEPLAYGLOBALS_H_
#define _TGAMEPLAYGLOBALS_H_


#define BOARD_WIDTH 160.0f
#define BOARD_HALFWIDTH 80.0f

#define PLAY_SCENE	"PLAY_SCENE"
#define PLAY_CAMERA "PLAY_CAMERA"

#define PLAY_BOARD_NODE		"PLAY_BOARD"
#define PLAY_BOARD_ENTITY	"BOARD_ENT"
#define PLAY_BOARD_MESH		"board.mesh"

#define BRICKZ_MESH "brick1.mesh"
#define BRICKZ_ENT	"BRICKZ_ENT"

#define BRICKI_MESH "brick2.mesh"
#define BRICKI_ENT	"BRICKI_ENT"
//...
enum BRICK_TYPE {
	BRICK_TYPE_T =1,
	BRICK_TYPE_Z =2,
	BRICK_TYPE_ZM=3,// mirrored Z
	BRICK_TYPE_L =4,
	BRICK_TYPE_LM=5,// mirrored L
	BRICK_TYPE_I =6,
	BRICK_TYPE_S =7
};
#endif