
#include "cLog.h"
#include "cPath.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

cPath::cPath()
{
	AStar = NULL;
	Done();
	
	nxf=-1;	//New direction initialization (false)
	nyf=-1;
}

cPath::~cPath(){}

void cPath::Make(int *map,int cx,int cy,int cxdest,int cydest)
{
	int status;		//find path?
	int ncx,ncy;	//next cell

	//Exists movement?
	if((cx!=cxdest)||(cy!=cydest))
	{
		world=map;
		AStar=new cAStar();
		AStar->InitializePathfinder();
		AStar->LoadMap(world);
		status=AStar->FindPath(1,cx,cy,cxdest,cydest);


		//Exists path?
		if(status)
		{
			x=cx;
			y=cy;
			xf=cxdest;
			yf=cydest;

			nxf=-1;	
			nyf=-1;

			//1st Direction
			AStar->NextCell(&ncx,&ncy);
			CalcDir(x,y,ncx,ncy);
		}
		else
		{
			//Delete A*
			if(AStar)
			{
				AStar->EndPathfinder();
				delete AStar;
				AStar = NULL;
			}
			//Reset trajectory settings
			Done();
			nxf=-1;
			nyf=-1;
		}
	}
}

void cPath::ReMake(int *map,int cxdest,int cydest)
{
	if(xf!=cxdest && yf!=cydest)
	{
		world=map;
		nxf=cxdest;
		nyf=cydest;
	}
}

int cPath::NextStep(int *px,int *py,int *cx,int *cy)
{
	int move=CONTINUE;

	switch(dir)
	{
		case N:	(*py)-=STEP_LENGTH;						break;
		case S:	(*py)+=STEP_LENGTH;						break;
		case E:	(*px)+=STEP_LENGTH;						break;
		case O:	(*px)-=STEP_LENGTH;						break;
		case NE:(*py)-=STEP_LENGTH; (*px)+=STEP_LENGTH; break;
		case NO:(*py)-=STEP_LENGTH; (*px)-=STEP_LENGTH; break;
		case SE:(*py)+=STEP_LENGTH; (*px)+=STEP_LENGTH; break;
		case SO:(*py)+=STEP_LENGTH; (*px)-=STEP_LENGTH; break;
	}

	//Calculate next cell
	if( (((*px)%32)==0) && (((*py)%32)==0))
	{
		x = (*px)>>5; *cx = x;
		y = (*py)>>5; *cy = y;

		if((nxf==-1) && (nyf==-1))
		{
			move=NextCell();
		}
		else//if((nxf>=0) || (nyf>=0))
		{
			AStar->EndPathfinder();
			delete AStar;
			AStar = NULL;
			Make(world,*cx,*cy,nxf,nyf);
			//move=CONTINUE;
		}
	}
	return move;
}

int cPath::NextCell()
{	
	int ncx,ncy;

	if((x==xf)&&(y==yf))
	{
		AStar->EndPathfinder();
		delete AStar;
		AStar = NULL;
		return ARRIVE;
	}
	else
	{
		AStar->NextCell(&ncx,&ncy);
		CalcDir(x,y,ncx,ncy);
		return CONTINUE;
	}
}

int cPath::Faced()
{
	return dir;
}
bool cPath::IsDone()
{
	return (dir == STOP);
}
void cPath::Done()
{
	dir = STOP;
}

void cPath::CalcDir(int x1,int y1,int x2,int y2)
{
	int sdx,sdy;	//sign movement

	sdx=x2-x1;
	sdy=y2-y1;

	// - Horitzontal
	if(sdy==0)
	{
		if(sdx>0) dir=E;
		else	  dir=O;
	}
	// - Vertical
	else if(sdx==0)
	{
		if(sdy>0) dir=S;
		else	  dir=N;
	}
	// - m=dx/dy=1
	else if(abs(sdx)==abs(sdy))
	{
		if(sdx>0)
		{
			if(sdy>0)	dir=SE;
			else		dir=NE;
		}
		else
		{
			if(sdy>0)	dir=SO;
			else		dir=NO;
		}
	}
}

