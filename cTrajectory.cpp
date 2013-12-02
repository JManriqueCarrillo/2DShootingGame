
#include "cTrajectory.h"
#include "cScene.h"
#include <math.h>

cTrajectory::cTrajectory()
{
	dir=STOP;
	
	nxf=-1;	//New direction initialization (false)
	nyf=-1;
}

cTrajectory::~cTrajectory(){}

void cTrajectory::Make(int cx,int cy,int cxdest,int cydest)
{
	int sdx,sdy;	//sign movement
	int dx,dy;		//increment

	x=cx;
	y=cy;
	xf=cxdest;
	yf=cydest;
	sdx=xf-x;
	sdy=yf-y;
	dx=abs(sdx);
	dy=abs(sdy);

	nxf=-1;	
	nyf=-1;
	dir=STOP;

	//Exists movement?
	if((cx!=cxdest)||(cy!=cydest))
	{
		//** trivial cases **

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

		//** no trivial cases ** <Midpoint algorithm (Bresenham's)>
		else
		{
			if(x<xf)
			{
				if(dx>dy)
				{
					p=(2*dy)-dx;
					k1=2*dy;
					k2=2*(dy-dx);
					if(y>yf) dir=C1;
					else	 dir=C8;
				}				
				else
				{
					p=(2*dx)-dy;
					k1=2*dx;
					k2=2*(dx-dy);
					if(y>yf) dir=C2;
					else	 dir=C7;
				}
			}
			else
			{				
				if(dx<dy)
				{
					p=(2*dx)-dy;
					k1=2*dx;
					k2=2*(dx-dy);
					if(y>yf) dir=C3;
					else	 dir=C6;
				}				
				else
				{
					p=(2*dy)-dx;
					k1=2*dy;
					k2=2*(dy-dx);
					if(y>yf) dir=C4;
					else	 dir=C5;
				}
			}
		}

		//Calculate first subdir
		switch(dir)
		{
			case C1: (p<=0) ? subdir=E : subdir=NE; break;
			case C2: (p<=0) ? subdir=N : subdir=NE; break;
			case C3: (p<=0) ? subdir=N : subdir=NO; break;
			case C4: (p<=0) ? subdir=O : subdir=NO; break;
			case C5: (p<=0) ? subdir=O : subdir=SO; break;
			case C6: (p<=0) ? subdir=S : subdir=SO; break;
			case C7: (p<=0) ? subdir=S : subdir=SE; break;
			case C8: (p<=0) ? subdir=E : subdir=SE; break;
			
			//Trivial case
			default: subdir=dir;
		}
	}
}

void cTrajectory::ReMake(int cxdest,int cydest)
{
	nxf=cxdest;
	nyf=cydest;
}

int cTrajectory::NextStep(int *px,int *py,int *cx,int *cy)
{
	int move=CONTINUE;

	switch(subdir)
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

	//Arrive? Calculate next cell (subdir) or remake
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
			Make(x,y,nxf,nyf);
		}
	}
	return move;
}

int cTrajectory::NextCell()
{
	if((x==xf)&&(y==yf)) return ARRIVE;
	else
	{
		switch(dir)
		{
			//case N,S,E,O,NE,NO,SE,SO same subdirection

			case C1:
					if(p<0)
					{
						p=p+k1;
						subdir=E;
					}
					else
					{
						p=p+k2;
						subdir=NE;
					}
					break;

			case C2:
					if(p<0)
					{
						p=p+k1;
						subdir=N;
					}
					else
					{
						p=p+k2;
						subdir=NE;
					}
					break;

			case C3:
					if(p<0)
					{
						p=p+k1;
						subdir=N;
					}
					else
					{
						p=p+k2;
						subdir=NO;
					}
					break;

			case C4:
					if(p<0)
					{
						p=p+k1;
						subdir=O;
					}
					else
					{
						p=p+k2;
						subdir=NO;
					}
					break;

			case C5:
					if(p<0)
					{
						p=p+k1;
						subdir=O;
					}
					else
					{
						p=p+k2;
						subdir=SO;
					}
					break;

			case C6:
					if(p<0)
					{
						p=p+k1;
						subdir=S;
					}
					else
					{
						p=p+k2;
						subdir=SO;
					}
					break;

			case C7:
					if(p<0)
					{
						p=p+k1;
						subdir=S;
					}
					else
					{
						p=p+k2;
						subdir=SE;
					}
					break;

			case C8:
					if(p<0) 
					{
						p=p+k1;
						subdir=E;
					}
					else
					{
						p=p+k2;
						subdir=SE;
					}
					break;
		}
		return CONTINUE;
	}
}

int cTrajectory::Faced()
{
	return subdir;
}
bool cTrajectory::IsDone()
{
	return (dir == STOP);
}
void cTrajectory::Done()
{
	dir = STOP;
}
