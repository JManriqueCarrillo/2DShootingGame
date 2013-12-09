
#ifndef __PATH_H__
#define __PATH_H__

#include "cAStar.h"

//Direction
#define N		1
#define S		2
#define E		3
#define O		4
#define NE		5
#define NO		6
#define SE		7
#define SO		8

//Going
#define STOP		0
#define CONTINUE	1
#define ARRIVE		2

//Step length
#define STEP_LENGTH	2	//Posible values = {1,2,4,8,16,32}


class cPath
{
public:
	cPath();
	virtual ~cPath();

	int x,y;	//actual cell
	int xf,yf;	//objective cell
	int nxf,nyf;//new objective cell (superposed case)
	int dir;	//direction
	cAStar *AStar;
	int *world;

	void Make(int *map,int cx,int cy,int cxdest,int cydest);	//Make new path
	void ReMake(int *map,int cxdest,int cydest);				//Make new path overlapping previous
	int  NextStep(int *px,int *py,int *cx,int *cy);				//Calculate next step position
	int  NextCell();										//Calculate next cell

	int  Faced();
	bool IsDone();
	void Done();

private:
	void CalcDir(int x1,int y1,int x2,int y2);
};

#endif
