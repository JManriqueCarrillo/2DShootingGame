
#ifndef __SCENE_H__
#define __SCENE_H__

//Resolution
#define SCREEN_RES_X	800
#define SCREEN_RES_Y	600

//Visible part
#define SCENE_WIDTH		25
#define SCENE_HEIGHT	19

//Dimension = Area^2
#define SCENE_AREA		32

//Map coordinate beginning
#define SCENE_Xo		0
#define SCENE_Yo		0
#define SCENE_Xf		( SCENE_Xo + (SCENE_WIDTH<<5) )
#define SCENE_Yf		( SCENE_Yo + (SCENE_HEIGHT<<5) )

//Radar position (left,top)
#define RADAR_Xo		672
#define RADAR_Yo		60
#define RADAR_Xf		800
#define RADAR_Yf		188

//Direction
#define Norte		1
#define Sur			2
#define Este		3
#define Oeste		4

class cScene
{

public:
	
	cScene();
	virtual ~cScene();

	void LoadMap(char *file);
	void GenerateMap();
	void Move(int pointer);
	void MoveMap(int dir);
	void getMoveMapOffsets(int *offsetx, int *offsety, int *offsetWIDTH, int *offsetHEIGHT);
	void getMovePlayerOffsets(float *offsetx, float *offsety);
	void MoveByRadar(int x,int y);
	bool isWalkable(int x, int y);
	bool Visible(int cellx,int celly);

	int map[SCENE_WIDTH][SCENE_HEIGHT];
	int auxMap[SCENE_WIDTH][SCENE_HEIGHT];
	int * Pathmap;
	int cx,cy;
	bool isMoving;
	int moveDir;
	int moveCounter;

	char logtext[100];
};

#endif
