
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
	GenerateMap();
}

int getDireccionContraria(int direccion)
{
	int retdir = direccion;

	switch(direccion)
	{
		case Norte: retdir = Sur; break;
		case Sur: retdir = Norte; break;
		case Este: retdir = Oeste; break;
		case Oeste: retdir = Este; break;
	}

	return retdir;
}

void cScene::GenerateMap()
{
	int y,x,n;
	int random;

	srand (time(NULL));

	// Ceiling, walls and doors
	for(y=0;y<SCENE_HEIGHT;y++)
	{
		for(x=0;x<SCENE_WIDTH;x++)
		{
			doorMap[x][y] = 0;
			walkMap[x][y] = 0;
			auxMap[x][y] = map[x][y];

			if(y==0 || y== SCENE_HEIGHT-1 || y== SCENE_HEIGHT-2 || x == 0 || x == SCENE_WIDTH-1)
				n = 0;
			else if(y==1)
				n = 1;
			else
			{
				n = 2;
				walkMap[x][y] = 1;
			}
			
			map[x][y]=n;
		}
	}
	
	// Floor random
	for(y=2;y<SCENE_HEIGHT-2;y++)
	{
		for(x=1;x<SCENE_WIDTH-1;x++)
		{
			random = rand() / (RAND_MAX / 10 + 1);
			if(random == 0)
				map[x][y]=3;
			else if(random == 1)
				map[x][y]=4;
			else if(random == 2)
				map[x][y]=5;
			else
				map[x][y]=2;
		}
	}

	// Stuff random
	for(y=3;y<SCENE_HEIGHT-3;y++)
	{
		for(x=2;x<SCENE_WIDTH-2;x++)
		{
			if(x==12 || x==12 || y==8 || y==9) continue;

			random = rand() / (RAND_MAX / 100 + 1);
			if(random == 0)
			{
				walkMap[x][y] = 0;
				map[x][y] = 50;
			}
			else if(random == 1)
			{
				walkMap[x][y] = 0;
				map[x][y] = 51;
			}
		}
	}

	// Generate the position of the doors
	do{
		random = 1 + rand() / (RAND_MAX / 4 + 1);
	}while(random == getDireccionContraria(moveDir));

	// Entrance door
	if(moveDir != 0)
	{
		switch(getDireccionContraria(moveDir))
		{
			case Norte: map[12][0] = 84; map[13][0] = 2; map[12][1] = 2; map[13][1] = 2; break;
			case Sur: map[12][17] = 24; map[13][17] = 2; map[12][18] = 2; map[13][18] = 2; break;
			case Oeste: map[0][8] = 44; map[0][9] = 2; break;
			case Este: map[24][8] = 64; map[24][9] = 2; break;
		}
	}

	// Exit door
	switch(random)
	{
		case Norte: map[12][0] = 80; map[13][0] = 2; map[12][1] = 2; map[13][1] = 2;
					doorMap[12][0] = Norte; doorMap[13][0] = Norte; doorMap[12][1] = Norte; doorMap[13][1] = Norte; break;
		case Sur: map[12][17] = 20; map[13][17] = 2; map[12][18] = 2; map[13][18] = 2; 
				  doorMap[12][17] = Sur; doorMap[13][17] = Sur; doorMap[12][18] = Sur; doorMap[13][18] = Sur; break;
		case Oeste: map[0][8] = 40; map[0][9] = 2;
					doorMap[0][8] = Oeste; doorMap[0][9] = Oeste; break;
		case Este: map[24][8] = 60; map[24][9] = 2;
				   doorMap[24][8] = Este; doorMap[24][9] = Este; break;
	}
	// The exit door is always close at start
	isDoorOpen = false;

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
				ClosePrevDoor();
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
	//if(map[x][y] == 0 || map[x][y] == 1) return false;
	if(walkMap[x][y] == 0) return false;

	return true;
}

bool cScene::Visible(int cellx,int celly)
{
	return ((cellx>=cx)&&(cellx<cx+SCENE_WIDTH)&&(celly>=cy)&&(celly<cy+SCENE_HEIGHT)) ? 1 : 0;
}

void cScene::OpenNextDoor()
{
	if(!isDoorClosing)
	{
		isDoorOpening = true;
		doorOpenCounter = 0;
	}
}

void cScene::ClosePrevDoor()
{
	if(!isDoorOpening)
	{
		isDoorClosing = true;
		doorCloseCounter = 0;
	}
}

void cScene::UpdateDoors()
{
	int n, ndoor;
	static int delay = 0;

	delay++;
	if(delay < 4) return;
	delay = 0;

	// Update opening doors
	if(isDoorOpening)
	{
		for(int y=0;y<SCENE_HEIGHT;y++)
		{
			for(int x=0;x<SCENE_WIDTH;x++)
			{
				n = map[x][y];
				ndoor = doorMap[x][y];
				if(((n >= 80 && n < 84) || (n >= 60 && n < 64) || (n >= 40 && n < 44) || (n >= 20 && n < 24)) && (ndoor == Norte || ndoor == Sur || ndoor == Este || ndoor == Oeste))
				{
					map[x][y]++;
				}
			}
		}

		doorOpenCounter++;
		if(doorOpenCounter >= 4)
		{
			isDoorOpening = false;
			isDoorOpen = true;

			for(int y=0;y<SCENE_HEIGHT;y++)
			{
				for(int x=0;x<SCENE_WIDTH;x++)
				{
					ndoor = doorMap[x][y];
					if(ndoor == Norte || ndoor == Sur || ndoor == Este || ndoor == Oeste)
					{
						walkMap[x][y] = 1;
					}
				}
			}
		}
	}
	// Update closing doors
	else if(isDoorClosing)
	{
		for(int y=0;y<SCENE_HEIGHT;y++)
		{
			for(int x=0;x<SCENE_WIDTH;x++)
			{
				n = map[x][y];
				if((n > 80 && n <= 84) || (n > 60 && n <= 64) || (n > 40 && n <= 44) || (n > 20 && n <= 24))
				{
					map[x][y]--;
				}
			}
		}

		doorCloseCounter++;
		if(doorCloseCounter >= 4)
		{
			isDoorClosing = false;
		}
	}
}