
#include "cScene.h"
#include "cMouse.h"
#include <stdio.h>
#include <time.h>

cScene::cScene()
{
	cx=0;
	cy=0;
	isMoving = false;
	moveDir = 0;
}
cScene::~cScene(){}

void cScene::LoadMap(char *file)
{
	/*int i,j,n;
	
	FILE *f;
	f=fopen("map.txt","r");

	for(i=0;i<SCENE_HEIGHT;i++)
	{
		for(j=0;j<SCENE_WIDTH;j++)
		{
			fscanf(f,"%d",&n);
			map[j][i]=n;
		}
	}

	fclose(f);*/
	GenerateMap();
}

void cScene::GenerateMap()
{
	int y,x,n;
	for(y=0;y<SCENE_HEIGHT;y++)
	{
		for(x=0;x<SCENE_WIDTH;x++)
		{
			auxMap[x][y] = map[x][y];

			if(y==0 || y== SCENE_HEIGHT-1 || y== SCENE_HEIGHT-2 || x == 0 || x == SCENE_WIDTH-1)
				n = 0;
			else if(y==1)
				n = 1;
			else
				n = 2;
			
			map[x][y]=n;
		}
	}
	srand (time(NULL));
	for(y=2;y<SCENE_HEIGHT-2;y++)
	{
		for(x=1;x<SCENE_WIDTH-1;x++)
		{
			if(rand() / (RAND_MAX / 10 + 1) == 0)
				map[x][y]=3;
			else
				map[x][y]=2;
		}
	}

	int random;

	do{
		random = 1 + rand() / (RAND_MAX / 4 + 1);
	}while(random == moveDir);

	switch(random)
	{
		case Norte: map[12][1] = 4; map[13][1] = 4; break;
		case Sur: map[12][17] = 5; map[13][17] = 5; break;
		case Oeste: map[0][8] = 6; map[0][9] = 6; break;
		case Este: map[24][8] = 7; map[24][9] = 7; break;
	}

	//Pathmap for the enemy
	Pathmap=(int *)malloc(sizeof(int)*(SCENE_HEIGHT*SCENE_WIDTH));

	int k=0;

	for(y=2;y<SCENE_HEIGHT-2;y++)
	{
		for(x=1;x<SCENE_WIDTH-1;x++)
		{
			
				Pathmap[k]=map[x][y];
				k++;
		}
	}

}

void cScene::MoveMap(int dir)
{
	int x,y;
	static bool isGenerated;
	int maxCounter;

	if(!isMoving)
	{
		moveDir = dir;
		moveCounter = 0;
		isMoving = true;
		isGenerated = false;
	}
	else
	{
		if(!isGenerated)
		{
			GenerateMap();
			isGenerated = true;
		}
		else
		{
			moveCounter++;

			switch(moveDir)
			{
				case Norte:
				case Sur:
					maxCounter = SCENE_HEIGHT;
					break;
				case Oeste:
				case Este:
					maxCounter = SCENE_WIDTH;
					break;
			}

			if(moveCounter >= maxCounter)
			{
				isMoving = false;
			}
		}
	}
}

void cScene::getMoveMapOffsets(int *offsetx, int *offsety, int *offsetWIDTH, int *offsetHEIGHT)
{
	switch(moveDir)
	{
		case Norte:
			*offsety += moveCounter;
			*offsetHEIGHT -= SCENE_HEIGHT;
			break;
		case Sur:
			*offsety -= moveCounter;
			*offsetHEIGHT += SCENE_HEIGHT;
			break;
		case Oeste:
			*offsetx += moveCounter;
			*offsetWIDTH -= SCENE_WIDTH;
			break;
		case Este:
			*offsetx -= moveCounter;
			*offsetWIDTH += SCENE_WIDTH;
			break;
	}
}

void cScene::getMovePlayerOffsets(float *offsetx, float *offsety)
{
	switch(moveDir)
	{
		case Norte:
			*offsety += float(SCENE_HEIGHT-5)/float(SCENE_HEIGHT);
			break;
		case Sur:
			*offsety -= float(SCENE_HEIGHT-6)/float(SCENE_HEIGHT);
			break;
		case Oeste:
			*offsetx += float(SCENE_WIDTH-4)/float(SCENE_WIDTH);
			break;
		case Este:
			*offsetx -= float(SCENE_WIDTH-4)/float(SCENE_WIDTH);
			break;
	}
}

void cScene::Move(int pointer)
{
	//map=32x32, visible=20x17 => move=0..32-20,0..32-17=0..12,0..15

	//Up
	if((pointer==MN)||(pointer==MNO)||(pointer==MNE))
	{
		if(cy>0) cy--;
	}
	//South
	else if((pointer==MS)||(pointer==MSO)||(pointer==MSE))
	{
		if(cy<SCENE_AREA-SCENE_HEIGHT) cy++;
	}
	//West
	if((pointer==MO)||(pointer==MNO)||(pointer==MSO))
	{
		if(cx>0) cx--;
	}
	//East
	else if((pointer==ME)||(pointer==MNE)||(pointer==MSE))
	{
		if(cx<SCENE_AREA-SCENE_WIDTH) cx++;
	}
}


void cScene::MoveByRadar(int x,int y)
{
	//move=0..12,0..15
	//rect=(80x68)/4=20x17

	cx=(x-RADAR_Xo)>>2;
	cy=(y-RADAR_Yo)>>2;

	if(cx<=9)		cx=0;
	else if(cx>=21)	cx=12;
	else			cx-=9;
	
	if(cy<=8)		cy=0;
	else if(cy>=23)	cy=15;
	else			cy-=8;
}

bool cScene::isWalkable(int x, int y)
{
	if(map[x][y] == 0 || map[x][y] == 1) return false;

	return true;
}

bool cScene::Visible(int cellx,int celly)
{
	return ((cellx>=cx)&&(cellx<cx+SCENE_WIDTH)&&(celly>=cy)&&(celly<cy+SCENE_HEIGHT)) ? 1 : 0;
}